#include "OctTree.h"
#include <assert.h>

Cell::Cell()
    : b(nullptr) {
}
Cell::Cell(bounding_box box)
    : bounds(box)
    , b(nullptr) {
}
bounding_box::bounding_box() {
}

bounding_box::bounding_box(const cord& pos1, const cord& pos2) {
    center = 0.5 * (pos2 + pos1);
    length = pos2[0] - pos1[0];
    if (length < 0) {
        length = -length;
    }
}
bool bounding_box::contains(const cord& position) {
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


void Cell::insert_body(const body& body) {
    assert(bounds.contains(body.pos));
    if (this->b == nullptr) {
        this->b = &body;
        return;
    }
    double half = bounds.length / 2;
    cord center = bounds.center;
    std::array<bounding_box, 8> child_boxes = {
        bounding_box(center, cord(half, half, half)),
        bounding_box(center, cord(half, half, -half)),
        bounding_box(center, cord(half, -half, half)),
        bounding_box(center, cord(half, -half, -half)),
        bounding_box(center, cord(-half, half, half)),
        bounding_box(center, cord(-half, half, -half)),
        bounding_box(center, cord(-half, -half, half)),
        bounding_box(center, cord(-half, -half, -half)),
    };
    for (int i = 0; i < 8; ++i) {
        bounding_box box = child_boxes[i];
        if (box.contains(body.pos)) {
            if (children[i] == nullptr) {
                children[i] = std::unique_ptr<Cell>(new Cell(box));
            }
            children[i]->insert_body(body);
        }
    }
}
