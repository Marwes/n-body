
#include <vector>
#include "body.h"


int main(int argc, char** argv) {
    const int numBodies = 100;
    const int iterations = 10;
    const double dt = 0.1;
    std::vector<body> bodies(numBodies);
    std::vector<vec> forces(numBodies);
    for (int ii = 0; ii < iterations; ++ii) {
        for (int bi = 0; bi < numBodies; ++bi) {
            vec force;
            const body& self = bodies[bi];
            for (int bj = 0; bj < numBodies; ++bj) {
                force += self.forceFrom(bodies[bj]);
            }
            forces[bi] = force;
        }
        for (int bi = 0; bi < numBodies; ++bi) {
            //bodies[bi].update(forces[bi]);
        }
    }
    return 0;
}
