#include "OctTree.h"
#include <assert.h>

Cell::Cell()
    : b(nullptr)
    , external(true) {
}
Cell::Cell(bounding_box box)
    : bounds(box)
    , b(nullptr)
    , external(true) {
}
bounding_box::bounding_box() {
}

bounding_box::bounding_box(const vec& pos1, const vec& pos2) {
    center = 0.5 * (pos2 + pos1);
    length = pos2[0] - pos1[0];
    if (length < 0) {
        length = -length;
    }
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

OctTree::OctTree(const std::vector<body>& bodies) {
    for (auto& body: bodies) {
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
    double half = bounds.length / 2;
    vec center = bounds.center;
    std::array<bounding_box, 8> child_boxes = {
        bounding_box(center, vec(half, half, half)),
        bounding_box(center, vec(half, half, -half)),
        bounding_box(center, vec(half, -half, half)),
        bounding_box(center, vec(half, -half, -half)),
        bounding_box(center, vec(-half, half, half)),
        bounding_box(center, vec(-half, half, -half)),
        bounding_box(center, vec(-half, -half, half)),
        bounding_box(center, vec(-half, -half, -half)),
    };
    for (int i = 0; i < child_boxes.size(); ++i) {
        bounding_box box = child_boxes[i];
        if (box.contains(newBody.pos)) {
            if (children[i] == nullptr) {
                children[i] = std::unique_ptr<Cell>(new Cell(box));
                external = false;
            }
            children[i]->insert_body(newBody);
        }
    }
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
        return true;
    });
    assert(bodies_in_tree == n_bodies);
}
