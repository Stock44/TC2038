/**
 * Actividad 3.4 Implementación de Kruskal
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This file contains an implementation of a Disjoint Set data structure, as well as of Kruskal's algorithm
 * for finding the minimum spanning forest / tree in an undirected graph represented as an adjacency matrix.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 12.2.1
 * g++ -o out -std=c++20 main.cpp
 * ./out
 *
 * 17/04/23
 */
#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>
#include <set>
#include <memory>

using AdjacencyMatrix = std::vector<std::vector<int>>;

// Data structure to store data about a particular edge
struct GraphEdge {
    std::size_t start;
    std::size_t end;
    int weight;
};

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
    explicit DisjointSet(std::size_t size) {
        for (std::size_t node = 0; node < size; node++) {
            nodes.emplace_back(std::make_unique<Node>(nullptr, 0));
            nodes.at(node)->parent = nodes.at(node).get();
        }
    }

    /**
     * Finds the representative of a given node, while also flattening the tree of the set, allowing for faster future
     * checks. Worst case, it runs in O(a(n)), with a() being the inverse Ackermann function, growing extremely slowly, and n
     * being the number of nodes.
     * @param node
     * @return the node's representative
     */
    Node *findRepresentative(std::size_t node) {
        auto currentNode = nodes.at(node).get();
        while (currentNode->parent != currentNode) {
            currentNode->parent = currentNode->parent->parent;
            currentNode = currentNode->parent;
        }
        return currentNode;
    }

    /**
     * Computes the union of the subsets of the given nodes, using the ranks as guide in order to prevent the tree from
     * becoming too tall. Runs in O(a(n)), with a() being the inverse Ackermann function, growing extremely slowly, and n
     * being the number of nodes. After a call to this function, both nodes' representatives will be the same.
     * @param first
     * @param second
     * @return
     */
    void nodeUnion(std::size_t first, std::size_t second) {
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

private:
    std::vector<std::unique_ptr<Node>> nodes;
};

/**
 * Finds the minimum spanning forest / tree for a given graph represented by an adjacency matrix. It utilizes a disjoint
 * set to improve node cycles lookup. Runs in O(E log E), dominated by the sorting of the edge list to make lookup of the
 * edge with the smallest weight run in O(1).
 * @param graph
 */
void printKruskalMinimumSpanningTree(AdjacencyMatrix const &graph) {
    size_t vertexCount = graph.size();
    std::vector<GraphEdge> edges;
    DisjointSet disjointSet(vertexCount);

    // only adds upper right half of graph
    for (std::size_t vertex = 0; vertex < vertexCount; vertex++) {
        for (std::size_t neighbor = vertex + 1; neighbor < vertexCount; neighbor++) {
            // don't add unconnected nodes
            if (graph.at(vertex).at(neighbor) < 0) continue;

            edges.emplace_back(vertex, neighbor, graph.at(vertex).at(neighbor));
        }
    }
    std::ranges::sort(edges, std::less<>(), &GraphEdge::weight);

    int maxLength = 0;
    for (auto const &edge: edges) {
        if (disjointSet.findRepresentative(edge.start) != disjointSet.findRepresentative(edge.end)) {
            disjointSet.nodeUnion(edge.start, edge.end);
            std::cout << edge.weight << "\n";
            maxLength += edge.weight;
        }
    }

    std::cout << maxLength << std::endl;
}

int main() {
    // Get graph data
    int graphDim;
    std::cin >> graphDim;

    AdjacencyMatrix graph(graphDim, std::vector<int>(graphDim));

    // Put graph data into adjacency matrix structure
    for (auto &row: graph) {
        for (auto &edge: row) {
            std::cin >> edge;
        }
    }

    std::cout << "\n";

    // Print the kruskal minimum spanning tree
    printKruskalMinimumSpanningTree(graph);

    return 0;
}