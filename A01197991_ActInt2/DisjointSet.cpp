//
// Created by hiram on 5/26/23.
//

#include "DisjointSet.hpp"

dst::DisjointSet::DisjointSet(std::size_t size) {
    for (std::size_t node = 0; node < size; node++) {
        nodes.emplace_back(std::make_unique<Node>(nullptr, 0));
        nodes.at(node)->parent = nodes.at(node).get();
    }
}

dst::DisjointSet::Node *dst::DisjointSet::findRepresentative(std::size_t node) {
    auto currentNode = nodes.at(node).get();
    while (currentNode->parent != currentNode) {
        currentNode->parent = currentNode->parent->parent;
        currentNode = currentNode->parent;
    }
    return currentNode;
}

void dst::DisjointSet::nodeUnion(std::size_t first, std::size_t second) {
    auto firstRepr = findRepresentative(first);
    auto secondRepr = findRepresentative(second);

    if (firstRepr == secondRepr) {
        return;
    }

    if (firstRepr->rank < secondRepr->rank) {
        std::swap(firstRepr, secondRepr);
    }

    secondRepr->parent = firstRepr;
    if (firstRepr->rank == secondRepr->rank) {
        firstRepr->rank++;
    }
}
