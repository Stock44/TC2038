#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include "geometry.hpp"

using AdjacencyMatrixRow = std::vector<std::optional<float>>;
using AdjacencyMatrix = std::vector<AdjacencyMatrixRow>;

int main() {
    std::size_t n;
    std::cin >> n;

    AdjacencyMatrix distanceGraph{n, AdjacencyMatrixRow(n)};
    AdjacencyMatrix flowGraph{n, AdjacencyMatrixRow(n)};
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
    for(std::size_t i = 0; i < n; i++) {
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

    // TODO implement the 4 solutions

    return 0;
}