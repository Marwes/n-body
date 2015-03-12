
#include <vector>
#include "body.h"
#include "OctTree.h"
#include "io.h"



int main(int argc, char** argv) {
    std::vector<body> bodies = readBodies("data/test_1");
    const int iterations = 10;
    const double dt = 0.1;
    for (int ii = 0; ii < iterations; ++ii) {
        OctTree tree(bodies);
        verifyTree(tree, bodies.size());
        for (body& b: bodies) {
            tree.depth_first([&] (const Cell& cell) {
                return true;
            });
        }
    }
    return 0;
}
