/**
 * Actividad 4.3 Implementación Polígonos Convexos aplicando geometría computacional.
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This program implements an algorithm to determine the smallest convex polygon that surrounds
 * set of points by applying the Graham's Scan algorithm.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 13.1.1
 * g++ -o out -std=c++20 main.cpp a01197991geometry.cpp
 * ./out
 *
 * 25/05/23
 */

#include "a01197991geometry.hpp"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

/**
 * This functions obtains the smallest convex hull for a given vector of points, via the Graham's Scan algorithm.
 * It operates in O(n log n) time.
 * @param points
 * @return
 */
std::deque<geo::Point> convexHull(std::vector<geo::Point> const &points) {
    std::vector<geo::Point> sortedPoints = points;
    auto p0 = std::ranges::min(sortedPoints, std::less<>(), [](geo::Point const &point) {
        return std::tuple(point.y, point.x);
    });
    std::ranges::sort(sortedPoints, [&p0](geo::Point const &p1, geo::Point const &p2) {
        auto p1theta = atan2(p1.y - p0.y, p1.x - p0.x);
        auto p2theta = atan2(p2.y - p0.y, p2.x - p0.x);
        return std::tuple(p1theta, geo::distance(p0, p1)) < std::tuple(p2theta, geo::distance(p0, p2));
    });
    std::deque<geo::Point> convexHull;

    for (auto &point: sortedPoints) {
        auto top = convexHull.end() - 1;
        auto nextToTop = top - 1;
        while (convexHull.size() > 1 && (top->x - nextToTop->x) * (point.y - nextToTop->y) -
                                        (top->y - nextToTop->y) * (point.x - nextToTop->x) <= 0) {
            convexHull.pop_back();
            top = convexHull.end() - 1;
            nextToTop = top - 1;
        }

        convexHull.emplace_back(point);
    }

    return convexHull;
}

int main() {
    std::size_t n;
    std::cin >> n;

    std::vector<geo::Point> points;
    for (std::size_t i = 0; i < n; i++) {
        geo::Point &point = points.emplace_back();
        std::cin >> point.x;
        std::cin >> point.y;
    }

    auto ch = convexHull(points);

    std::cout << "\n";
    for (auto const &point: ch) {
        std::cout << point.x << " " << point.y << "\n";
    }

    std::cout.flush();

    return 0;
}