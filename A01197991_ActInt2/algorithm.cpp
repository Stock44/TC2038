//
// Created by hiram on 5/26/23.
//

#include "algorithm.hpp"
#include <queue>
#include <unordered_set>
#include <stack>
#include <numeric>

namespace alg {

    std::vector<GraphEdge> minimumSpanningTreeEdges(const AdjacencyMatrix<int> &graph) {
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

    AdjacencyMatrix<int> floydShortestPaths(const AdjacencyMatrix<int> &graph) {
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

    int maximumFlow(AdjacencyMatrix<int> const &capacityGraph, std::size_t v1, std::size_t v2) {
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

    int sendFlowHelper(AdjacencyMatrix<int> const &capacityGraph, AdjacencyMatrix<int> &flowGraph, std::size_t v,
                       std::size_t vt,
                       int flow,
                       std::vector<std::optional<std::size_t>> const &levels) {
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

}
