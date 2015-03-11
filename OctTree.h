#pragma once
#include <array>
#include <vector>
#include <memory>

#include "units.h"
#include "Body.h"

struct bounding_box {
    bounding_box();
    //Create a bounding box from two opposing corners
    bounding_box(const vec& pos1, const vec& pos2);

    //Check if the box contains the `position`
    bool contains(const vec& position);
    vec center;
    double length;
};

class Cell {
public:
    Cell();
    Cell(bounding_box box);

    void insert_body(const body& body);

    template<typename F>
    void depth_first(const F& f) {
        for (auto& child: children) {
            if (child != nullptr && (f(*child))) {
                child->depth_first(f);
            }
        }
    }
private:
    friend class OctTree;
    bounding_box bounds;
    const body* b;
    std::array<std::unique_ptr<Cell>, 8> children;
};

class OctTree {
public:
    OctTree(const std::vector<body>& bodies);
    template<typename F>
    void depth_first(const F& f) {
        cell.depth_first(f);
    }
private:
    Cell cell;
};
