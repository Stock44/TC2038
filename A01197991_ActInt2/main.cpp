#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include "geometry.hpp"
#include "algorithm.hpp"

int main() {
    int maxLength = 0;
    std::size_t n;
    std::cin >> n;
    alg::AdjacencyMatrix distanceGraph{n, alg::AdjacencyMatrixRow(n)};
    alg::AdjacencyMatrix flowGraph{n, alg::AdjacencyMatrixRow(n)};
    std::vector<geo::Point> points;

    for (std::size_t i = 0; i < n; i++) {
        auto &row = distanceGraph.at(i);
        for (std::size_t j = 0; j < n; j++) {
            float weight;
            std::cin >> weight;
            if (weight != 0) row.at(j) = weight;
        }
    }

    for (std::size_t i = 0; i < n; i++) {
        auto &row = flowGraph.at(i);
        for (std::size_t j = 0; j < n; j++) {
            float weight;
            std::cin >> weight;
            if (weight != 0) row.at(j) = weight;
        }
    }

    std::string line;
    for (std::size_t i = 0; i < n; i++) {
        std::getline(std::cin, line);
        std::string token;
        double x;
        double y;

        std::getline(std::cin, token, '(');
        std::getline(std::cin, token, ',');
        x = std::stod(token);
        std::getline(std::cin, token, ')');
        y = std::stod(token);
        points.emplace_back(x, y);
    }

    // execute all the algorithms here
    // minimum spanning tree to find most efficient way to connect all nodes
    auto mst = alg::minimumSpanningTreeEdges(distanceGraph);

    // traveling salesman problem to find path that goes through all nodes
    // TODO process floyd weights
    auto weights = alg::floydShortestPaths(distanceGraph);
    std::vector<std::pair<std::size_t, std::size_t>> shortestRoute;

    //assume distance graph is symmetrical

    // TODO find maximum flux for network

    // TODO find voronoi diagram

    // do all the printing here
    std::cout << "-1" << "\n";
    for (auto const &edge: mst) {
        std::cout << edge.weight << "\n";
        maxLength += edge.weight;
    }
    std::cout << maxLength << std::endl;

    std::cout << "-2" << "\n";


    std::cout.flush();

    return 0;
}