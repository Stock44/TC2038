/**
 * This file implements several algorithms used for the Actividad Integradora 2
 */

#pragma once

#include <vector>
#include <optional>
#include <algorithm>
#include <span>
#include <queue>
#include "DisjointSet.hpp"


namespace alg {
    template<typename T>
    using AdjacencyMatrixRow = std::vector<std::optional<T>>;
    template<typename T>
    using AdjacencyMatrix = std::vector<AdjacencyMatrixRow<T>>;

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
    std::vector<GraphEdge> minimumSpanningTreeEdges(AdjacencyMatrix<int> const &graph) {
        size_t vertexCount = graph.size();
        std::vector<GraphEdge> edges;
        dst::DisjointSet disjointSet(vertexCount);

        // only adds upper right half of graph
        for (std::size_t vertex = 0; vertex < vertexCount; vertex++) {
            for (std::size_t neighbor = vertex + 1; neighbor < vertexCount; neighbor++) {
                // don't add unconnected nodes
                if (!graph.at(vertex).at(neighbor).has_value()) continue;

                edges.emplace_back(vertex, neighbor, graph.at(vertex).at(neighbor).value());
            }
        }
        std::ranges::sort(edges, std::less<>(), &GraphEdge::weight);

        std::vector<GraphEdge> finalEdges;
        for (auto const &edge: edges) {
            if (disjointSet.findRepresentative(edge.start) != disjointSet.findRepresentative(edge.end)) {
                disjointSet.nodeUnion(edge.start, edge.end);
                finalEdges.push_back(edge);
            }
        }
        return finalEdges;
    }

    /**
     * Implementation of Floyd's Shortest Paths algorithm for a graph represented by an adjacency matrix. It runs
     * in O(V^3) time, executing three loops within each other from 0 to V.
     * @param graph
     * @return
     */
    AdjacencyMatrix<int> floydShortestPaths(AdjacencyMatrix<int> const &graph) {
        auto n = graph.size();
        AdjacencyMatrix<int> pathWeights = graph;

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (!pathWeights[i][k].has_value() || !pathWeights[k][j].has_value()) continue;
                    if (!pathWeights[i][j].has_value() ||
                        pathWeights[i][j].value() > pathWeights[i][k].value() + pathWeights[k][j].value()) {
                        pathWeights[i][j] = pathWeights[i][k].value() + pathWeights[k][j].value();
                    }
                }
            }
        }
        return pathWeights;
    }

    /**
     * Recursive helper function for the Dinic's maximum flow algorithm.
     * @param capacityGraph
     * @param flowGraph
     * @param v
     * @param vt
     * @param flow
     * @param levels
     * @return
     */
    int sendFlowHelper(AdjacencyMatrix<int> const &capacityGraph, AdjacencyMatrix<int> &flowGraph, std::size_t v,
                       std::size_t vt, int flow, std::vector<std::optional<std::size_t>> const &levels){
        if (v == vt) return flow;

        for (std::size_t vi = 0; vi < capacityGraph.size(); vi++) {
            if (levels.at(vi) == levels.at(v).value() + 1 && flowGraph.at(v).at(vi) < capacityGraph.at(v).at(vi)) {
                int currentFlow = std::min(flow,
                                           capacityGraph.at(v).at(vi).value() - flowGraph.at(v).at(vi).value());

                int tempFlow = sendFlowHelper(capacityGraph, flowGraph, vi, vt, currentFlow, levels);

                if (tempFlow <= 0) continue;

                flowGraph.at(v).at(vi).value() += tempFlow;

                flowGraph.at(vi).at(v).value() -= tempFlow;
                return tempFlow;
            }
        }

        return 0;
    }

    /**
     * Obtains the maximum flow across a graph represented by an adjacency matrix. It runs in O(V^2 E) time, exploring
     * the graph using breadth-first search and propagating flow recursively throughout the graph's edges and vertices.
     * @param graph
     * @param v1
     * @param v2
     * @return
     */
    int maximumFlow(AdjacencyMatrix<int> const &capacityGraph, std::size_t v1, std::size_t v2){
        AdjacencyMatrix<int> flowGraph{capacityGraph.size(), AdjacencyMatrixRow<int>(capacityGraph.size(), 0)};

        std::vector<std::optional<std::size_t>> levels(capacityGraph.size());
        levels.at(v1) = 0;

        std::vector<GraphEdge> edges;

        if (v1 == v2) return 0;

        int total = 0;

        while (true) {
            for (auto &level: levels) {
                level = {};
            }

            levels.at(v1) = 0;

            std::queue<std::size_t> queue;
            queue.push(v1);

            while (!queue.empty()) {
                std::size_t u = queue.front();
                queue.pop();

                for (std::size_t to = 0; to < capacityGraph.size(); to++) {
                    if (u == to) continue;
                    if (!levels.at(to).has_value() && flowGraph.at(u).at(to) < capacityGraph.at(u).at(to)) {
                        levels.at(to) = levels.at(u).value() + 1;

                        queue.push(to);
                    }
                }
            }

            if (!levels.at(v2).has_value()) break;

            while (true) {
                int flow = sendFlowHelper(capacityGraph, flowGraph, v1, v2, std::numeric_limits<int>::max(), levels);

                if (flow == 0) break;

                total += flow;
            }
        }

        return total;
    }
}