#pragma once
#include <array>
#include <vector>
#include <memory>

#include "units.h"
#include "body.h"

class Cell {
public:

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
    double length;
    cord center;
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
