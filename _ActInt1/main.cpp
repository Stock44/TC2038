#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <string>
#include <sstream>

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
    std::vector<std::ifstream> transmissionFiles;
    transmissionFiles.emplace_back("transmission1.txt");
    transmissionFiles.emplace_back("transmission2.txt");

    std::vector<std::string> transmissions;

    for (auto &file: transmissionFiles) {
        std::string &transmission = transmissions.emplace_back();
        std::stringstream transmissionStream{transmission};
        std::string transmissionLine;

        for (std::string line; std::getline(file, line);) {
            transmissionStream << transmissionLine;
        }
    }


    std::vector<std::ifstream> maliciousCodes;
    maliciousCodes.emplace_back("mcode1.txt");
    maliciousCodes.emplace_back("mcode2.txt");
    maliciousCodes.emplace_back("mcode3.txt");

    return 0;
}