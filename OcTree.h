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

//Enumeration which indicates wether a traversal should continue into the children
//nodes of a cell
enum class Traverse {
    Continue,
    Stop
};

class Cell {
public:
    Cell();
    Cell(Cell&& other);
    //Creates a cell bounded by `box`
    Cell(bounding_box box);
    Cell& operator=(Cell&& other);

    //Inserts a body into this cell, it is assumed that the body is within the bounds
    //of this cell
    void insert_body(const body& body);
    //Gets the subcell which encompasses `pos`.
    //Assumed that the `pos` is within this cell
    Cell& getCell(const vec& pos);
    //Returns wether this flag is external (no children)
    bool is_external() const;

    //Returns the body that this cell holds or null if holds no body
    const body* getBody() const {
        return b;
    }

    //Returns the center of mass for this cell
    const vec& getCenterOfMass() const {
        return massCenter;
    }

    //Returns the mass of this cell (the sum of the mass of each subcell)
    double getMass() const {
        return mass;
    }

    //Returns the side length of this cell
    double getLength() const {
        return bounds.length;
    }

    //Does a depth first this cell. If the function `f` returns `Continue`
    //It will continue calling `preorderWalk` on each of its children
    template<typename F>
    void preorderWalk(const F& f) {
        switch (f(*this)) {
            case Traverse::Continue:
                for (auto& child: children) {
                    if (child != nullptr) {
                        child->preorderWalk(f);
                    }
                }
                break;
            case Traverse::Stop:
                break;
        }
    }
private:
    friend class OcTree;
    bool external;
    bounding_box bounds;
    vec massCenter;
    double mass;
    const body* b;
    std::array<std::unique_ptr<Cell>, 8> children;
};

//An octree which partitions space into 8 cells at each level in the tree
//Inserting each body into its own external cell.
//It is assumed that the `bodies` vector is not resized while the tree is alive
class OcTree {
public:
    //Constructs an Oct
    OcTree(const std::vector<body>& bodies, bounding_box bounds);

    vec forceOnBody(const double sqTheta, const body& self);

    template<typename F>
    void preorderWalk(const F& f) {
        cell.preorderWalk(f);
    }
    const bounding_box& getBounds() const { return cell.bounds; }
private:
    Cell cell;
};

//Verifies that the tree is correct to some extent
void verifyTree(OcTree& tree, int n_bodies);
