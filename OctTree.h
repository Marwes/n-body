#pragma once
#include <iosfwd>
#include <array>
#include <vector>
#include <memory>

#include "units.h"
#include "body.h"

struct bounding_box {
    bounding_box();
    //Create a bounding box from two opposing corners
    bounding_box(const vec& pos1, const vec& pos2);

    //Check if the box contains the `position`
    bool contains(const vec& position);
    vec center;
    double length;
};

std::ostream& operator << (std::ostream& o, const bounding_box& b);

enum class Traverse {
    Continue,
    Stop
};

class Cell {
public:
    Cell();
    Cell(Cell&& other);
    Cell(bounding_box box);
    Cell& operator=(Cell&& other);

    void insert_body(const body& body);
    Cell& getCell(const vec& pos);
    bool is_external() const;

    const body* getBody() const {
        return b;
    }

    const vec& getCenterOfMass() const {
        return massCenter;
    }

    double getMass() const {
        return mass;
    }

    double getLength() const {
        return bounds.length;
    }

    template<typename F>
    void depth_first(const F& f) {
        switch (f(*this)) {
            case Traverse::Continue:
                for (auto& child: children) {
                    if (child != nullptr) {
                        child->depth_first(f);
                    }
                }
                break;
            case Traverse::Stop:
                break;
        }
    }
private:
    void insert_body(Cell& cell, const body& newBody);
    friend class OctTree;
    bool external;
    bounding_box bounds;
    vec massCenter;
    double mass;
    const body* b;
    std::array<std::unique_ptr<Cell>, 8> children;
};

class OctTree {
public:
    OctTree(const std::vector<body>& bodies, bounding_box bounds);
    template<typename F>
    void depth_first(const F& f) {
        cell.depth_first(f);
    }
    const bounding_box& getBounds() const { return cell.bounds; }
    int bodies_out_of_bounds;
private:
    Cell cell;
};

void verifyTree(OctTree& tree, int n_bodies);
