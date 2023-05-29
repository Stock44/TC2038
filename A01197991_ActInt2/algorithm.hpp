//
// Created by hiram on 5/26/23.
//

#pragma once

#include <vector>
#include <optional>
#include <algorithm>
#include <span>
#include "DisjointSet.hpp"


namespace alg {
    using AdjacencyMatrixRow = std::vector<std::optional<float>>;
    using AdjacencyMatrix = std::vector<AdjacencyMatrixRow>;

    // Data structure to store data about a particular edge
    struct GraphEdge {
        std::size_t start;
        std::size_t end;
        float weight;
    };

    /**
     * Finds the minimum spanning forest / tree for a given graph represented by an adjacency matrix. It utilizes a disjoint
     * set to improve node cycles lookup. Runs in O(E log E), dominated by the sorting of the edge list to make lookup of the
     * edge with the smallest weight run in O(1).
     * @param graph
     */
    std::vector<GraphEdge> minimumSpanningTreeEdges(AdjacencyMatrix const &graph);

    /**
     * Este algoritmo calcula las distancias mas cortas para un grafo representado por una matriz de adjacencia. El algoritmo
     * se ejecuta en tiempo O(V^3), realizando tres bucles desde 0 a V.
     * @param graph La matriz de adjacencia que representa el grafo.
     * @return Una matriz de adjacencia que continene las distancias minimas entre vertices.
     */
    AdjacencyMatrix floydShortestPaths(AdjacencyMatrix const &graph);

    bool bfs(AdjacencyMatrix const &graph, std::size_t v1, std::size_t v2);

    int sendFlow(AdjacencyMatrix const &graph, std::size_t v1, std::size_t v2, int flow, int t, std::span<std::size_t> start);

    float maximumFlow(AdjacencyMatrix const &graph, std::size_t v1, std::size_t v2);
}