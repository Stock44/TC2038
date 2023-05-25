/**
 * Actividad 4.2 Implementación búsqueda geométrica
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This program implements a divide and conquer algorithm for finding the two closest points within a list
 * of points, in 2D space.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 13.1.1
 * g++ -o out -std=c++20 a01197991main.cpp a01197991geometry.cpp
 * ./out
 *
 * 18/05/23
 */

#include "a01197991geometry.hpp"
#include <span>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

using PointPairDistance = std::tuple<geo::Point, geo::Point, double>;

/**
 * Finds the closest points within a range of points, using euclidean distance. This runs in O(n^2), as each node
 * has to be compared with each other node.
 * @param points
 * @return
 */
PointPairDistance bruteForce(std::span<geo::Point> points) {
    double min = std::numeric_limits<float>::max();
    std::size_t point1 = 0;
    std::size_t point2 = 1;
    for (std::size_t i = 0; i < points.size(); ++i)
        for (std::size_t j = i + 1; j < points.size(); ++j)
            if (geo::distance(points[i], points[j]) < min) {
                point1 = i;
                point2 = j;
                min = geo::distance(points[i], points[j]);
            }

    return {points[point1], points[point2], min};
}

/**
 * Finds the closest set of points that cross a strip, with a minimum distance. Runs in O(n) time, inner loop doesn't run
 * for every element.
 * @param strip
 * @param d
 * @return
 */
PointPairDistance stripClosest(std::span<geo::Point> strip, double d) {
    double min = d;
    std::size_t point1 = 0;
    std::size_t point2 = 1;

    std::ranges::sort(strip, std::less<>(), &geo::Point::y);

    for (int i = 0; i < strip.size(); ++i)
        for (int j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < min; ++j)
            if (geo::distance(strip[i], strip[j]) < min) {
                point1 = i;
                point2 = j;
                min = geo::distance(strip[i], strip[j]);
            }

    return {strip[point1], strip[point2], min};
}

/**
 * Internal helper function for a divide and conquer algorithm for finding the closest pair of
 * points of a set of points.
 * @tparam R range type for range of points
 * @param points range of points
 * @return
 */
template<std::ranges::contiguous_range R>
PointPairDistance closestPointsHelper(R points) {
    if (points.size() <= 3)
        return bruteForce(points);

    std::size_t mid = points.size() / 2;
    geo::Point &midPoint = points[mid];

    auto [leftP1, leftP2, leftMinDist] = closestPointsHelper(points | std::views::take(mid));
    auto [rightP1, rightP2, rightMinDist] = closestPointsHelper(points | std::views::drop(mid));

    double minDist = std::min(leftMinDist, rightMinDist);
    geo::Point minP1{};
    geo::Point minP2{};
    if (leftMinDist < rightMinDist) {
        minP1 = leftP1;
        minP2 = leftP2;
    } else {
        minP1 = rightP1;
        minP2 = rightP2;
    }

    auto stripView = points | std::views::filter([&midPoint, minDist](geo::Point const &p) {
        return std::abs(p.x - midPoint.x) < minDist;
    });
    std::vector<geo::Point> strip(std::ranges::begin(stripView), std::ranges::end(stripView));

    auto [stripP1, stripP2, stripMinDist] = stripClosest(strip, minDist);

    if (stripMinDist < minDist) {
        return {stripP1, stripP2, stripMinDist};
    }

    return {minP1, minP2, minDist};
}

/**
 * Finds the closest pair of points in a list of points in O(n log n log n), with the helper function having a
 * complexity of O(n log n) and the sorting function O(log n).
 * @param points all points
 * @return
 */
std::tuple<geo::Point, geo::Point, double> closestPoints(std::span<geo::Point> points) {
    std::ranges::sort(points, std::less<>(), &geo::Point::x);
    return closestPointsHelper(points);
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
    auto [p1, p2, distance] = closestPoints(points);
    std::cout << std::round(distance) << "\n";
    if (std::tuple(p1.x, p1.y) < std::tuple(p2.x, p2.y)) {
        std::cout << p1.x << " " << p1.y << "\n";
        std::cout << p2.x << " " << p2.y << "\n";
    } else {
        std::cout << p2.x << " " << p2.y << "\n";
        std::cout << p1.x << " " << p1.y << "\n";
    }
    return 0;
}