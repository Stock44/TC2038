/**
 * Actividad Integradora 2
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * Compiled on Arch Linux (Linux 6.3.4) with the gcc compiler toolchain version 13.1.1
 * g++ -o out -std=c++20 main.cpp
 * ./out
 *
 * 30/03/2023
 */
#include <vector>
#include <iostream>
#include "geometry.hpp"
#include "algorithm.hpp"

int main() {
    int maxLength = 0;
    std::size_t n;
    std::cin >> n;
    alg::AdjacencyMatrix<int> distanceGraph{n, alg::AdjacencyMatrixRow<int>(n)};
    alg::AdjacencyMatrix<int> flowGraph{n, alg::AdjacencyMatrixRow<int>(n)};
    std::vector<geo::Point> points;

    for (std::size_t i = 0; i < n; i++) {
        auto &row = distanceGraph.at(i);
        for (std::size_t j = 0; j < n; j++) {
            int weight;
            std::cin >> weight;
            if (weight != 0) row.at(j) = weight;
        }
    }

    for (std::size_t i = 0; i < n; i++) {
        auto &row = flowGraph.at(i);
        for (std::size_t j = 0; j < n; j++) {
            int weight;
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

        std::cin >> x;
        std::cin >> y;
        points.emplace_back(x, y);
    }

    // execute all the algorithms here
    // minimum spanning tree to find most efficient way to connect all nodes
    auto mst = alg::minimumSpanningTreeEdges(distanceGraph);

    // traveling salesman problem to find path that goes through all nodes
    // TODO process floyd weights
    auto [path, pathWeight] = alg::travellingSalesman(distanceGraph, 0);

    //assume distance graph is symmetrical

    // TODO find maximum flux for network
    int maxFlow = alg::maximumFlow(flowGraph, 0, flowGraph.size() - 1);

    auto voronoi = geo::voronoiDiagram(points);

    std::ranges::sort(voronoi, [](auto const &p1, auto const &p2) {
        double min1 = std::ranges::min(p1.points, std::less(), &geo::Point::x).x;
        double min2 = std::ranges::min(p2.points, std::less(), &geo::Point::x).x;
        return min1 < min2;
    });

    for (auto &polygon: voronoi) {
        auto minIt = std::ranges::min_element(polygon.points, std::less(), &geo::Point::x);

        if (minIt != polygon.points.end()) std::ranges::rotate(polygon.points, minIt + 1);
    }

    // Print the minimum expansion tree
    std::cout << "-1" << "\n";
    for (auto const &edge: mst) {
        std::cout << edge.weight << "\n";
        maxLength += edge.weight;
    }
    std::cout << maxLength << std::endl;

    // Print the traveling salesman solution
    std::cout << "-2" << "\n";
    for (auto vertex: path) {
        std::cout << vertex << " ";
    }
    std::cout << "\n" << pathWeight << "\n";

    // Print the maximum flow
    std::cout << "-3" << "\n";
    std::cout << maxFlow << "\n";

    // Print the voronoi polygons
    std::cout << "-4" << "\n";
    int count = 1;
    for (auto const &polygon: voronoi) {
        std::cout << "-4" << count << "\n";
        for (auto const &point: polygon.points | std::views::reverse) {
            std::cout << std::round(point.x) << " " << std::round(point.y) << "\n";
        }
        count++;
    }

    std::cout.flush();

    return 0;
}