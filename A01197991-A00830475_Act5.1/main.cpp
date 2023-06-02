/**
 * Actividad 5.1 Implementación backtracking con Bitmask
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This file implements a basic backtracking algorithm that uses bit masks to improve time complexity.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 12.2.1
 * g++ -o out -std=c++20 main.cpp
 * ./out
 *
 * 01/06/23
 */
#include <iostream>
#include <vector>
#include <cstdint>

using PersonCapContainers = std::vector<std::vector<std::size_t>>;
using uint_64 = std::uint_fast64_t;
using CapMask = std::pair<uint_64, uint_64>;

/**
 * Recursive helper for the algorithm
 * @param personCapContainers cap id containers, per person
 * @param capIndex
 * @param caps
 * @param mask
 */
void backtrackingHelper(PersonCapContainers const &personCapContainers, PersonCapContainers &combinations,
                        std::size_t capIndex = 0,
                        std::vector<std::size_t> const &caps = {},
                        CapMask mask = {}) {
    if (caps.size() >= personCapContainers.size()) {
        combinations.push_back(caps);

        return;
    }

    if (capIndex >= personCapContainers.at(caps.size()).size()) return;

    auto const &personCaps = personCapContainers.at(caps.size());
    auto capId = personCaps.at(capIndex);
    auto newCaps = caps;
    newCaps.push_back(capId);

    CapMask newMask = mask;
    bool valid;
    if (capId > 64) {
        capId -= 64;
        valid = !(1 << capId & newMask.second);
        newMask.second |= 1 << capId;
    } else {
        valid = !(1 << capId & newMask.first);
        newMask.first |= 1 << capId;
    }
    if (valid) backtrackingHelper(personCapContainers, combinations, 0, newCaps, newMask);

    backtrackingHelper(personCapContainers, combinations, capIndex + 1, caps, mask);

}

/**
 * Find all possible combinations where no Cap IDs repeat, in O(2^n log n)
 * @param personCapContainers
 * @return
 */
PersonCapContainers backtracking(PersonCapContainers const &personCapContainers) {
    PersonCapContainers combinations;
    backtrackingHelper(personCapContainers, combinations);

    return combinations;
}

int main() {
    std::size_t personCount;
    std::cin >> personCount;
    PersonCapContainers personCapContainers(personCount);

    for (auto &personCaps: personCapContainers) {
        std::size_t capCount;
        std::cin >> capCount;
        personCaps.reserve(capCount);
        for (std::size_t j = 0; j < capCount; j++) {
            std::size_t capId;
            std::cin >> capId;
            personCaps.push_back(capId);
        }
    }

    std::cout << "\n";

    auto combinations = backtracking(personCapContainers);
    std::cout << combinations.size() << "\n";
    for (auto const &combination: combinations) {
        for (auto id: combination) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }

    return 0;
}