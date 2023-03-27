/**
 * Actividad 2.2 Implementación de "Suffix Array"
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 * Este archivo contiene una implementación de un algoritmo para generar un arreglo de sufijos a partir de una cadena
 * de entrada.
 * 02/03/2023
 */

#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <iostream>

/**
 * Generates a suffix array in n (log n)^2 time
 * @param inputString
 * @return
 */
std::vector<std::string> generateSuffixArray(std::string_view inputString) {
    using std::vector;
    using std::tuple;


    vector<vector<int>> suffixRankings;
    vector<tuple<int, int, int>> tuples(inputString.length());

    suffixRankings.emplace_back();
    suffixRankings.at(0).reserve(inputString.length());
    for (char strChar: inputString) {
        suffixRankings.at(0).push_back(strChar - 'a');
    }

    for (int i = 1; std::pow(2, i - 1) < inputString.length(); i++) {
        suffixRankings.emplace_back(inputString.length());
        auto prevRanking = suffixRankings.at(i - 1);

        for (int j = 0; j < inputString.length(); j++) {
            auto index = j;
            auto first = prevRanking.at(j);
            int second;

            float nextIndex = j + std::pow(2, i - 1);
            if (nextIndex < inputString.length()) {
                second = prevRanking.at(static_cast<int>(nextIndex));
            } else {
                second = -1;
            }
            tuples.at(j) = {first, second, index};
        }
        std::ranges::sort(tuples);
        for (int j = 0; j < inputString.length(); j++) {
            auto [first, second, index] = tuples.at(j);

            int ranking;

            if (j > 0) {
                auto [prevFirst, prevSecond, prevIndex] = tuples.at(j - 1);
                if (first == prevFirst && second == prevSecond) ranking = suffixRankings.at(i).at(prevIndex);
                else ranking = j;

            } else ranking = j;

            suffixRankings.at(i).at(index) = ranking;
        }
    }


    vector<std::string> suffixArray(inputString.length());
    auto ranking = *(suffixRankings.end() - 1);
    for (int i = 0; i < inputString.length(); i++) {
        suffixArray.at(ranking.at(i)) = inputString.substr(i);
    }


    return suffixArray;
}


int main() {

    std::string inputString;

    std::cin >> inputString;

    auto suffixArray = generateSuffixArray(inputString);

    for (auto const &suffix: suffixArray) {
        std::cout << suffix << "\n";
    }

    std::cout.flush();

    return 0;
}


