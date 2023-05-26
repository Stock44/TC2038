//
// Created by hiram on 5/26/23.
//

#include "algorithm.hpp"
#include <queue>
#include <unordered_set>
#include <stack>

namespace alg {

    std::vector<std::pair<std::size_t, std::size_t>> minimumSpanningTreeEdges(const AdjacencyMatrix &graph) {
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


        std::vector<std::pair<std::size_t, std::size_t>> treeEdges;
        for (auto const &edge: edges) {
            if (disjointSet.findRepresentative(edge.start) != disjointSet.findRepresentative(edge.end)) {
                disjointSet.nodeUnion(edge.start, edge.end);
                treeEdges.emplace_back(edge.start, edge.end);
            }
        }
        return treeEdges;
    }

    AdjacencyMatrix floydShortestPaths(const AdjacencyMatrix &graph) {
        auto n = graph.size();
        AdjacencyMatrix pathWeights = graph;

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

    bool bfs(const AdjacencyMatrix &graph, std::size_t v1, std::size_t v2) {
        for (int i = 0; i < graph.size(); i++) {

        }
        return false;
    }

    int sendFlow(const AdjacencyMatrix &graph, std::size_t v1, std::size_t v2, int flow, int t,
                 std::span<std::size_t> start) {
        return 0;
    }

    float maximumFlow(AdjacencyMatrix const &capacityGraph, std::size_t v1, std::size_t v2) {
        std::vector<std::vector<float>> flowGraph{capacityGraph.size(), std::vector<float>(v, 0.0f)};

        std::vector<std::optional<std::size_t>> levels{capacityGraph.size()};
        levels.at(v1) = 0;

        std::vector<GraphEdge> edges;

        if (v1 == v2) return 0;
        int total = 0;

        while (true) {
            std::queue<std::size_t> q;
            q.push(v1);

            while (!q.empty()) {
                std::size_t u = q.front();
                q.pop();

                for (std::size_t neighbor = 0; neighbor < capacityGraph.size(); neighbor++) {
                    auto capacity = capacityGraph.at(u).at(neighbor);
                    // if not connected skip
                    if (!capacity.has_value()) continue;
                    auto flow = flowGraph.at(u).at(neighbor);

                    if (!levels.at(neighbor).has_value() && flow < capacity.value()) {
                        levels.at(neighbor) = levels.at(u).value() + 1;

                        q.push(neighbor);
                    }
                }
            }

            // if destination vertex can not be reached
            if (!levels.at(v2).has_value()) break;

            std::unordered_set<std::size_t> visitedNodes{capacityGraph.size()};

//            while (true) {
//                std::stack<float> flowStack;
//                while(true) {
//                }
//            }
        }

    }

}
