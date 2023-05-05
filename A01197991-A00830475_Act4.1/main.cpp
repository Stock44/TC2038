/**
 * Actividad 4.1 Implementación Intersección y proximidad aplicando geometría computacional.
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This file implements the solution to activity 4.1, finding whether an pairs of lines
 * intersect each other.
 *
 * 04/05/23
 */

#include "geometry.hpp"
#include <iostream>
#include <deque>

int main() {
    std::size_t n;

    std::cin >> n;

    std::deque<bool> results;
    for (std::size_t i = 0; i < n / 4; i++) {
        geo::LineSegment l1{};
        geo::LineSegment l2{};

        std::cin >> l1.p1.x >> l1.p1.y >> l1.p2.x >> l1.p2.y;
        std::cin >> l2.p1.x >> l2.p1.y >> l2.p2.x >> l2.p2.y;

        results.push_back(geo::intersect(l1, l2));
    }

    for (auto result: results) {
        std::cout << (result ? "true" : "false") << "\n";
    }

    std::cout.flush();

    return 0;
}