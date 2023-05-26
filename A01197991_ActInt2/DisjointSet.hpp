//
// Created by hiram on 5/26/23.
//

#pragma once

#include <vector>
#include <memory>

namespace dst {
    /**
    * A disjoint set contains a collection of subsets of a set. It allows fast location of a "representative" of a given
    * subset, enabling checking whether two elements are in the same subset. It also allows for the union of the subsets of
    * any two elements quickly. It is represented internally as an array of trees of pointers, with all nodes stored
    * in an array and each holding a reference to its parent, as well as a "rank", used for flattening the tree.
    * The root nodes hold references to themselves.
    */
    class DisjointSet {
    public:
        struct Node {
            Node *parent;
            int rank;
        };

        /**
         * Creates a disjoint set of a given size, with each element being initialized in its own subset in O(n) time.
         * @param size
         */
        explicit DisjointSet(std::size_t size);

        /**
         * Finds the representative of a given node, while also flattening the tree of the set, allowing for faster future
         * checks. Worst case, it runs in O(a(n)), with a() being the inverse Ackermann function, growing extremely slowly, and n
         * being the number of nodes.
         * @param node
         * @return the node's representative
         */
        Node *findRepresentative(std::size_t node);

        /**
         * Computes the union of the subsets of the given nodes, using the ranks as guide in order to prevent the tree from
         * becoming too tall. Runs in O(a(n)), with a() being the inverse Ackermann function, growing extremely slowly, and n
         * being the number of nodes. After a call to this function, both nodes' representatives will be the same.
         * @param first
         * @param second
         * @return
         */
        void nodeUnion(std::size_t first, std::size_t second);

    private:
        std::vector<std::unique_ptr<Node>> nodes;
    };
}
