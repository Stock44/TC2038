#include <vector>
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

        std::cin >> x;
        std::cin >> y;
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


    // Print the voronoi polygons
    std::cout << "-4" << "\n";
    int count = 1;
    for (auto const &polygon: voronoi) {
        std::cout << "-4" << count << "\n";
        for (auto const &point: polygon.points | std::views::reverse) {
            std::cout << point.x << " " << point.y << "\n";
        }
        count++;
    }

    std::cout.flush();

    return 0;
}