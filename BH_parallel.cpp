
#include <iostream>
#include <vector>
#include "body.h"
#include "OcTree.h"
#include "io.h"
#include "debug.h"

void runBH_parallel(std::vector<body>& bodies, const int iterations, const double theta, const int workers, const double dt) {
    std::vector<vec> forces(bodies.size());
    vec max_position;
    for (const body& b: bodies) {
        for (int i = 0; i < DIM; ++i) {
            if (std::abs(b.pos[i]) > max_position[i])
                max_position[i] = std::abs(b.pos[0]);
        }
    }
    bounding_box bounds = bounding_box(-3 * max_position, 3 * max_position);
    const double sqTheta = theta * theta;
    #pragma omp parallel
    for (int ii = 0; ii < iterations; ++ii) {
        DPRINT("Begin iteration " << ii);
        OcTree tree(bodies, bounds);
        #pragma omp for
        for (int bi = 0; bi < bodies.size(); ++bi) {
            forces[bi] = tree.forceOnBody(sqTheta, bodies[bi]);
        }
        #pragma omp for
        for (int bi = 0; bi < bodies.size(); ++bi) {
            bodies[bi].update(forces[bi], dt);
        }
        //Cache the bounds for the next iteration to avoid unnecessary resizing
        bounds = tree.getBounds();
    }
}

int main(int argc, char** argv) {
    NBodyRunner(argc, argv, "output_BHparallel", true, true, runBH_parallel);
    return 0;
}
