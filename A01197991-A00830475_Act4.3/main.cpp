/**
 * Actividad 4.3 Implementación Polígonos Convexos aplicando geometría computacional.
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This program implements an algorithm to determine the smallest convex polygon in a
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


int main() {
    std::size_t n;
    std::cin >> n;
    std::vector<geo::Point> points;
    for (std::size_t i = 0; i < n; i++) {
        geo::Point &point = points.emplace_back();
        std::cin >> point.x;
        std::cin >> point.y;
    }


    return 0;
}