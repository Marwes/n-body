
#include <vector>
#include "body.h"
#include "OctTree.h"


int main(int argc, char** argv) {
    std::vector<body> bodies;
    const int iterations = 10;
    const double dt = 0.1;
    for (int ii = 0; ii < iterations; ++ii) {
        OctTree tree(bodies);
        for (body& b: bodies) {
            
        }
    }
    return 0;
}
