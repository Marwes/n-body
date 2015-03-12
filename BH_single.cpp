
#include <iostream>
#include <vector>
#include "body.h"
#include "OctTree.h"
#include "io.h"


int main(int argc, char** argv) {
    std::vector<body> bodies = readBodies("data/test_1", TEXT_MODE);
    std::vector<vec> forces(bodies.size());
    std::cerr << bodies.size() << " bodies" << std::endl;
    const int iterations = 10;
    const double dt = 0.1;
    const double theta = 0.5;
    for (int ii = 0; ii < iterations; ++ii) {
        std::cerr << "Begin iteration " << ii << std::endl;
        OctTree tree(bodies, bounding_box(vec(0, 0, 0), vec(100, 100, 100)));
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
    }
    return 0;
}
