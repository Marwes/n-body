#include "OctTree.h"
#include <assert.h>
#include <iostream>
#include "debug.h"

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

Cell& Cell::getCell(const vec& pos) {
    assert(bounds.contains(pos));
    double half = bounds.length / 2;
    vec center = bounds.center;
    std::array<bounding_box, 8> child_boxes = {
        bounding_box(center, center + vec(half, half, half)),
        bounding_box(center, center + vec(half, half, -half)),
        bounding_box(center, center + vec(half, -half, half)),
        bounding_box(center, center + vec(half, -half, -half)),
        bounding_box(center, center + vec(-half, half, half)),
        bounding_box(center, center + vec(-half, half, -half)),
        bounding_box(center, center + vec(-half, -half, half)),
        bounding_box(center, center + vec(-half, -half, -half)),
    };
    for (int i = 0; i < child_boxes.size(); ++i) {
        bounding_box box = child_boxes[i];
        if (box.contains(pos)) {
            if (children[i] == nullptr) {
                children[i] = std::unique_ptr<Cell>(new Cell(box));
                external = false;
            }
            return *children[i];
        }
    }
    assert(false);
}
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

double sign(double x) {
    if (x > 0) return 1;
    else return -1;
}

OctTree::OctTree(const std::vector<body>& bodies, bounding_box bounds)
    : cell(bounds) {
    for (auto& body: bodies) {
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
            DPRINT(cell.bounds << " " << body.pos);
            assert(cell.bounds.contains(body.pos));
        }
        cell.insert_body(body);
    }
}

void Cell::insert_body(Cell& cell, const body& newBody) {
    //c => center of mass for current
    //cN => center of mass for cell N
    //wN => masses for cell N
    //c = (c1 * w1 + c2 * w2) / (w1 + w2)
    //c * (w1 + w2) - c2 * w2 = c1 * w1
    //m = w1 + w2
    //c' = c1 * w1 + c2' * w2'
    vec center = getCenterOfMass() * getMass() - cell.getMass() * cell.getCenterOfMass();
    double mass = getMass() - cell.getMass();
    cell.insert_body(newBody);
    this->massCenter = center + cell.getMass() * cell.getCenterOfMass();
    this->mass = mass + cell.getMass();

    //Do it the easy way to make sure it is correct
    this->mass = 0;
    this->massCenter = vec();
    for (auto& child: children) {
        if (child != nullptr) {
            this->mass += child->getMass();
            this->massCenter += child->getMass() * child->getCenterOfMass();
        }
    }
    this->massCenter = this->massCenter / this->getMass();
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
    this->insert_body(getCell(newBody.pos), newBody);
    if (this->b != nullptr) {
        //Since this node is no longer external (we just inserted another body)
        //we need to remove the body this cell owned and re insert it
        assert(!this->is_external());
        const body* b = this->b;
        this->b = nullptr;
        insert_body(*b);
    }
}

bool Cell::is_external() const {
    return external; 
}

void verifyTree(OctTree& tree, int n_bodies) {
    int bodies_in_tree = 0;
    tree.depth_first([&] (const Cell& cell) {
        assert(cell.is_external() || (!cell.is_external() && cell.getBody() == nullptr));
        if (cell.getBody() != nullptr) {
            bodies_in_tree += 1;
        }
        return Traverse::Continue;
    });
    assert(bodies_in_tree == n_bodies);
}
