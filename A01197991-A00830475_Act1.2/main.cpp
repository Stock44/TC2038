/**
 * Actividad 1.2 Implementación de la técnica de programación "Programación dinámica" y "algoritmos avaros"
 * Implements a change counter using two different algorithms: a greedy one and a dynamic programming one.
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 * 23/02/2023
 */

#include <cmath>
#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>

/**
 * Splits a given quantity of currency into its allowed denominations using a dynamic programming algorithm
 * @param denominations Array of allowed currency denominations to use
 * @param quantity Total quantity to split into denominations
 * @return Vector of quantities
 */
std::vector<int> dynamicMoneyExchange(std::vector<int> const &denominations, int quantity) {
    auto coinQuantities = std::vector<int>(std::size(denominations));

    auto quantityTable = std::vector<std::vector<int>>(std::size(denominations));

    for (int idx = 0; idx < std::size(denominations); idx++) {
        auto denomination = denominations.at(idx);
        auto &row = quantityTable.at(idx);

        for (int amount = 0; amount <= quantity; amount++) {
            if (idx == 0) {
                row.push_back(std::floor(amount / denomination));
                continue;
            }

            if (amount < denomination) {
                row.push_back(quantityTable.at(idx - 1).at(amount));
                continue;
            }

            auto upper = quantityTable.at(idx - 1).at(amount);
            auto prev = row.at(amount - denomination) + 1;

            row.push_back(prev < upper ? prev : upper);
        }
    }
    auto rowIdx = std::distance(std::begin(quantityTable),
                                std::ranges::min_element(quantityTable, [](auto lhsRow, auto rhsRow) {
                                    return *(lhsRow.end() - 1) < *(rhsRow.end() - 1);
                                }));
    auto amount = quantity;


    while (amount != 0) {
        auto row = quantityTable.at(rowIdx);
        auto cell = row.begin() + amount;

        if (rowIdx != 0) {
            auto upperRow = quantityTable.at(rowIdx - 1);
            auto upperCell = upperRow.begin() + amount;
            if (*upperCell < *cell) {
                rowIdx -= 1;
                continue;
            }
        }

        coinQuantities.at(rowIdx) += amount / denominations.at(rowIdx);
        amount -= coinQuantities.at(rowIdx) * denominations.at(rowIdx);
        rowIdx -= 1;
    }
    return coinQuantities;
}

/**
 * Splits a given amount of currency into its allowed denominations using a greedy algorithm
 * @param denominations
 * @param quantity
 * @return
 */
std::vector<int> greedyMoneyExchange(std::vector<int> const &denominations, int quantity) {
    auto coinQuantities = std::vector<int>(std::size(denominations));
    for (int i = std::size(denominations) - 1; i > -1; i--) {
        int amount = std::floor(quantity / denominations.at(i));
        quantity -= amount * denominations.at(i);
        coinQuantities.at(i) = amount;
    }
    return coinQuantities;
}

int main() {
    int denominationCount;
    int productPrice;
    int cashReceived;

    std::cin >> denominationCount;

    std::vector<int> denominations(denominationCount);

    for (int i = 0; i < denominationCount; i++) {
        std::cin >> denominations.at(i);
    }

    std::cin >> productPrice;
    std::cin >> cashReceived;

    int change = cashReceived - productPrice;

    auto dynamicQuantities = dynamicMoneyExchange(denominations, change);
    auto greedyQuantities = greedyMoneyExchange(denominations, change);

    for (auto const &quantity: dynamicQuantities) {
        std::cout << quantity << "\n";
    }

    for (auto const &quantity: greedyQuantities) {
        std::cout << quantity << "\n";
    }

    return 0;
}
