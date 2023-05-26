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