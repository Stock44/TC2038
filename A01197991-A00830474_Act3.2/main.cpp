#include <iostream>
#include <vector>
#include <optional>
#include <queue>
#include <set>
#include <numeric>

using AdjacencyMatrix = std::vector<std::vector<int>>;
using Distances = std::vector<std::optional<int>>;
using PreviousVertices = std::vector<std::optional<int>>;

std::pair<Distances, PreviousVertices> djikstraShortesPath(AdjacencyMatrix const &graph, std::size_t source) {
    Distances distances(graph.size());
    PreviousVertices prevVertices(graph.size());
    std::queue<std::size_t> unexploredVertices;

    distances.at(source) = 0;
    unexploredVertices.push(source);

    std::set<std::size_t> exploredVertices;

    while (!unexploredVertices.empty()) {
        auto vertex = unexploredVertices.front();
        unexploredVertices.pop();
        exploredVertices.emplace(vertex);

        for (std::size_t neighbor = 0; neighbor < graph.at(vertex).size(); neighbor++) {
            // If the neighbor has already been explored or it isn't actually connected, ignore
            if (exploredVertices.contains(neighbor)) continue;
            if (graph.at(vertex).at(neighbor) == -1) continue;


            int distance = distances[vertex].value() + graph.at(vertex).at(neighbor);

            if (!distances[neighbor].has_value() || distance < distances.at(neighbor).value()) {
                distances.at(neighbor) = distance;
                prevVertices.at(neighbor) = vertex;
            }

            unexploredVertices.emplace(neighbor);
        }
    }

    return {std::move(distances), std::move(prevVertices)};
}

AdjacencyMatrix floydShortestPaths(AdjacencyMatrix const &graph) {
    auto n = graph.size();
    AdjacencyMatrix pathWeights = graph;

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (pathWeights[i][k] == -1 || pathWeights[k][j] == -1) continue;
                if (pathWeights[i][j] == -1 || pathWeights[i][j] > pathWeights[i][k] + pathWeights[k][j]) {
                    pathWeights[i][j] = pathWeights[i][k] + pathWeights[k][j];
                }
            }
        }
    }
    return pathWeights;
}

int main() {
    int graphDim;
    std::cin >> graphDim;

    AdjacencyMatrix graph(graphDim, std::vector<int>(graphDim));

    for (auto &row: graph) {
        for (auto &edge: row) {
            std::cin >> edge;
        }
    }

    std::vector<Distances> allDistances(graphDim);
    std::vector<PreviousVertices> allPrevVertices(graphDim);

    for (std::size_t vertex = 0; vertex < graphDim; vertex++) {
        auto [distances, prevVertices] = djikstraShortesPath(graph, vertex);

        allDistances.at(vertex) = std::move(distances);
        allPrevVertices.at(vertex) = std::move(prevVertices);
    }

    for (std::size_t vertex = 0; vertex < graphDim; vertex++) {
        for (std::size_t neighbor = 0; neighbor < graphDim; neighbor++) {
            if (vertex == neighbor) continue;
            auto distance = allDistances.at(vertex).at(neighbor);
            if (!distance) continue;
            std::cout << "node " << vertex + 1 << " to node " << neighbor + 1 << " : "
                      << distance.value() << "\n";
        }
    }

    auto pathWeights = floydShortestPaths(graph);
    for (auto const &row: pathWeights) {
        for (auto weight: row) {
            std::cout << weight << " ";
        }
        std::cout << "\n";
    }

    std::cout.flush();

    return 0;
}