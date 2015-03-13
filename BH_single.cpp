
#include <iostream>
#include <vector>
#include "body.h"
#include "OctTree.h"
#include "io.h"
#include "debug.h"


int main(int argc, char** argv) 
{
    std::vector<body> bodies = readBodies("data/test_1", TEXT_MODE);
    vec max_position;
    for (const body& b: bodies) {
        for (int i = 0; i < DIM; ++i) {
            if (std::abs(b.pos[i]) > max_position[i])
                max_position[i] = std::abs(b.pos[0]);
        }
    }
    std::vector<vec> forces(bodies.size());
    DPRINT(bodies.size() << " bodies");
    const int iterations = 100;
    const double dt = 0.1;
    const double theta = 0.5;
    bounding_box bounds = bounding_box(-3 * max_position, 3 * max_position);
    for (int ii = 0; ii < iterations; ++ii) {
        DPRINT("Begin iteration " << ii);
        OctTree tree(bodies, bounds);
        verifyTree(tree, bodies.size());
        for (int bi = 0; bi < bodies.size(); ++bi) {
            body& self = bodies[bi];
            vec force;
            tree.depth_first([&] (const Cell& cell) {
                //If the cell is external and holds a body we calculate the force
                //from that body directly
                if (cell.is_external()) {
                    if (cell.getBody() != nullptr) {
                        force += self.forceFrom(*cell.getBody());
                    }
                    return Traverse::Stop;
                }
                else {
                    //If the cell is internal and the cell is far enough away we can
                    //approximate the force by using the cells mass and center of mass
                    double h = cell.getLength() / (self.pos - cell.getCenterOfMass()).norm();
                    if (h < theta) {
                        force += self.forceFrom(cell.getCenterOfMass(), cell.getMass());
                        return Traverse::Stop;
                    }
                    else {
                        //Not far enough so continue into smaller cells
                        return Traverse::Continue;
                    }
                }
            });
            forces[bi] = force;
        }
        for (int bi = 0; bi < bodies.size(); ++bi) {
            bodies[bi].update(forces[bi], dt);
        }
        //Cache the bounds for the next iteration to avoid unnecessary resizing
        bounds = tree.getBounds();
    }
	writeBodies("output_BHsingle", 0, bodies);
    return 0;
}
