
#include <iostream>
#include <vector>
#include "body.h"
#include "OctTree.h"
#include "io.h"


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
    std::cerr << bodies.size() << " bodies" << std::endl;
    const int iterations = 100;
    const double dt = 0.1;
    const double theta = 0.5;
    bounding_box bounds = bounding_box(-3 * max_position, 3 * max_position);
    for (int ii = 0; ii < iterations; ++ii) {
        std::cerr << "Begin iteration " << ii << std::endl;
        OctTree tree(bodies, bounds);
        verifyTree(tree, bodies.size());
        for (int bi = 0; bi < bodies.size(); ++bi) {
            body& self = bodies[bi];
            vec force;
            tree.depth_first([&] (const Cell& cell) {
                if (cell.is_external() && cell.getBody() != nullptr) {
                    force += self.forceFrom(*cell.getBody());
                }
                else if (!cell.is_external() && false) {//Angle
                    force += self.forceFrom(cell.getCenterOfMass(), cell.getMass());
                    return false;
                }
                return true;
            });
            forces[bi] = force;
        }
        for (int bi = 0; bi < bodies.size(); ++bi) {
            bodies[bi].update(forces[bi], dt);
        }
        bounds = tree.getBounds();
    }
	writeBodies("output_BHsingle", 0, bodies);
    return 0;
}
