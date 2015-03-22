#include "OcTree.h"
#include <assert.h>
#include <iostream>
#include "debug.h"

bounding_box::bounding_box() {
}

bounding_box::bounding_box(const vec& pos1, const vec& pos2) {
    center = 0.5 * (pos2 + pos1);
    length = pos2[0] - pos1[0];
    if (length < 0) {
        length = -length;
    }
    assert(length > 0);
}
bool bounding_box::contains(const vec& position) {
    double halfSide = length / 2;
    for (int i = 0; i < DIM; ++i) {
        if (center[i] - halfSide > position[i]
         || center[i] + halfSide < position[i]) {
            return false;
        }
    }
    return true;
}

std::ostream& operator << (std::ostream& o, const bounding_box& b)
{
    o << "(" << b.center << ", " << b.length;
    return o; 
}

Cell::Cell()
    : Cell(bounding_box()) {
}
Cell::Cell(bounding_box box)
    : bounds(box)
    , b(nullptr)
    , external(true)
    , massCenter(box.center)
    , mass(0) {
}
Cell::Cell(Cell&& other)
    : b(other.b)
    , bounds(std::move(other.bounds))
    , external(other.external)
    , children(std::move(other.children)) {
}
Cell& Cell::operator=(Cell&& other) {
    b = other.b;
    mass = other.mass;
    massCenter = std::move(other.massCenter);
    bounds = std::move(other.bounds);
    external = other.external;
    children = std::move(other.children);
    return *this;
}

//Get the box at index i relative the center of the current cell
static bounding_box getBox(int i, const vec& center, double half) {
    switch (i) {
        case 0: return bounding_box(center, center + vec(half, half, half));
        case 1: return bounding_box(center, center + vec(half, half, -half));
        case 2: return bounding_box(center, center + vec(half, -half, half));
        case 3: return bounding_box(center, center + vec(half, -half, -half));
        case 4: return bounding_box(center, center + vec(-half, half, half));
        case 5: return bounding_box(center, center + vec(-half, half, -half));
        case 6: return bounding_box(center, center + vec(-half, -half, half));
        case 7: return bounding_box(center, center + vec(-half, -half, -half));
        default: assert(0);
    }
}

Cell& Cell::getCell(const vec& pos) {
    assert(bounds.contains(pos));
    double half = bounds.length / 2;
    vec center = bounds.center;
    vec dir = pos - center;
    //Choose the cell by creating an index out the direction which the position is relative the current center
    int i = (std::signbit(dir[0]) << 2) + (std::signbit(dir[1]) << 1) + std::signbit(dir[2]);
    assert(i < children.size());
    if (children[i] == nullptr) {
        bounding_box box = getBox(i, center, half);
        children[i] = std::unique_ptr<Cell>(new Cell(box));
        external = false;
    }
    return *children[i];
}

double sign(double x) {
    if (x > 0) return 1;
    else return -1;
}

OcTree::OcTree(const std::vector<body>& bodies, bounding_box bounds)
    : cell(bounds) {
    for (auto& body: bodies) {
        //If the octree does not contain the body we need to make the tree larger.
        //We do this by creating a larger cell which contains the current cell and then we place the current
        //cell in the correct place in the new cell
        if (!cell.bounds.contains(body.pos)) {
            DPRINT("Increase size of " << cell.bounds << " for " << body.pos); 
            vec direction = body.pos - bounds.center;
            direction[0] = sign(direction[0]);
            direction[1] = sign(direction[1]);
            direction[2] = sign(direction[2]);
            DPRINT("Direction " << direction);
            vec corner1 = cell.bounds.center - (cell.bounds.length / 2) * direction;
            vec corner2 = corner1 + cell.bounds.length * 2 * direction;

            Cell newCell(bounding_box(corner1, corner2));
            std::swap(cell, newCell);
            cell.getCell(newCell.bounds.center) = std::move(newCell);
        }
        if (!cell.bounds.contains(body.pos)) {
            std::cerr << cell.bounds << " " << body.pos << std::endl;
            assert(cell.bounds.contains(body.pos));
        }
        cell.insert_body(body);
    }
}

void Cell::insert_body(const body& newBody) {
    assert(bounds.contains(newBody.pos));
    //If this is any external cell with no body we can insert it directly
    if (this->b == nullptr && this->is_external()) {
        this->b = &newBody;
        this->massCenter = newBody.pos;
        this->mass = newBody.m;
        return;
    }
    //Otherwise we have the get the sub-cell which contains the body and insert it into that cell
    getCell(newBody.pos).insert_body(newBody);
    massCenter = (mass * massCenter + newBody.m * newBody.pos) / (mass + newBody.m);
    mass += newBody.m;
    if (this->b != nullptr) {
        //Since this node is no longer external (we just inserted another body)
        //we need to remove the body this cell owned and re insert it
        assert(!this->is_external());
        const body* b = this->b;
        this->b = nullptr;
        getCell(b->pos).insert_body(*b);
    }
}

vec OcTree::forceOnBody(const double sqTheta, const body& self) {
#ifdef DEBUG
    int nearBodies = 0;
#endif
    vec force;
    this->preorderWalk([&] (const Cell& cell) {
        //If the cell is external and holds a body we calculate the force
        //from that body directly
        if (cell.is_external()) {
            if (cell.getBody() != nullptr) {
#ifdef DEBUG
                nearBodies += 1;
#endif
                force += self.forceFrom(*cell.getBody());
            }
            return Traverse::Stop;
        }
        else {
            //If the cell is internal and the cell is far enough away we can
            //approximate the force by using the cells mass and center of mass
            vec pos_diff = self.pos - cell.getCenterOfMass();
            double distance = pos_diff.sqnorm();
            double h = cell.getLength() * cell.getLength() / distance;
            if (h < sqTheta) {
                force += calcForce(pos_diff, std::sqrt(distance), self.m, cell.getMass());
                return Traverse::Stop;
            }
            else {
                //Not far enough so continue into smaller cells
                return Traverse::Continue;
            }
        }
    });
    DPRINT("Near: " << nearBodies);
    return force;
}

bool Cell::is_external() const {
    return external; 
}

void verifyTree(OcTree& tree, int n_bodies) {
    int bodies_in_tree = 0;
    tree.preorderWalk([&] (const Cell& cell) {
        assert(cell.is_external() || (!cell.is_external() && cell.getBody() == nullptr));
        if (cell.getBody() != nullptr) {
            bodies_in_tree += 1;
        }
        return Traverse::Continue;
    });
    assert(bodies_in_tree == n_bodies);
}
