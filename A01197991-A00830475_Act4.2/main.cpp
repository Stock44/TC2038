/**
 * Actividad 4.2 Implementación búsqueda geométrica
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 13.1.1
 * g++ -o out -std=c++20 main.cpp geometry.cpp
 * ./out
 *
 * 18/05/23
 */

#include "geometry.hpp"
#include <span>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

using PointPairDistance = std::tuple<geo::Point, geo::Point, double>;

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