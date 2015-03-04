#include "OctTree.h"


OctTree::OctTree(const std::vector<body>& bodies) {
    for (auto& body: bodies) {
        cell.insert_body(body);
    }
}


void Cell::insert_body(const body& body) {
}
