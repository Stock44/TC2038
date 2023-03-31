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

    auto inputLength = inputString.length();

    vector<vector<int>> suffixRankings;
    vector<tuple<int, int, int>> tuples(inputLength);

    suffixRankings.emplace_back();
    suffixRankings.at(0).reserve(inputLength);
    for (char strChar: inputString) {
        suffixRankings.at(0).push_back(strChar - 'a');
    }

    for (std::size_t i = 1; static_cast<std::size_t>(std::pow(2, i - 1)) < inputLength; i++) {
        suffixRankings.emplace_back(inputLength);
        auto prevRanking = suffixRankings.at(i - 1);

        for (std::size_t j = 0; j < inputLength; j++) {
            auto index = j;
            auto first = prevRanking.at(j);
            std::size_t second;

            auto nextIndex = j + static_cast<std::size_t>(std::pow(2, i - 1));
            if (nextIndex < inputLength) {
                second = prevRanking.at(static_cast<int>(nextIndex));
            } else {
                second = -1;
            }
            tuples.at(j) = {first, second, index};
        }
        std::sort(tuples.begin(), tuples.end());
        for (int j = 0; j < inputLength; j++) {
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

std::string findLongestPalindrome(std::string const &text) {
    auto inputLength = text.length();

    if (inputLength == 0) return "";

    std::string new_text;
    for (int i = 0; i < inputLength * 2; i++) {
        new_text += text[i / 2];
    }

    const auto n = new_text.length();
    std::vector<std::size_t> p{n, 0};

    std::size_t center = 0;
    std::size_t right = 0;
    std::size_t maxCenter = 0;
    std::size_t maxLen = 0;

    for (int i = 1; i < n - 1; i++) {
        auto mirr = 2 * center - i;
        if (i < right) {
            p[i] = std::min(right - i, p[mirr]);
        }
        while (new_text[i + p[i] + 1] == new_text[i - p[i] - 1]) {
            p[i]++;
        }
        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }
        if (p[i] > maxLen) {
            maxLen = p[i];
            maxCenter = i;
        }
    }
    auto inicio = (maxCenter - maxLen) / 2;
    return text.substr(inicio, maxLen);
}

std::string readWholeFile(std::ifstream &file) {
    std::string fileContents;

    for (std::string line; std::getline(file, line);) {
        fileContents.append(line);
        fileContents.append("\n");
    }

    return fileContents;
}

int main() {
    std::vector<std::ifstream> transmissionFiles;
    std::vector<std::string> transmissions;
    std::vector<std::vector<std::string>> suffixArrays;

    std::vector<std::ifstream> maliciousCodeFiles;
    std::vector<std::string> maliciousCodes;

    transmissionFiles.emplace_back("transmission1.txt");
    transmissionFiles.emplace_back("transmission2.txt");

    maliciousCodeFiles.emplace_back("mcode1.txt");
    maliciousCodeFiles.emplace_back("mcode2.txt");
    maliciousCodeFiles.emplace_back("mcode3.txt");

    for (auto &file: transmissionFiles) {
        std::string &transmission = transmissions.emplace_back(readWholeFile(file));
        suffixArrays.emplace_back(generateSuffixArray(transmission));
    }

    std::transform(maliciousCodeFiles.begin(), maliciousCodeFiles.end(), std::back_inserter(maliciousCodes), readWholeFile);

    for (auto const &suffixArray: suffixArrays) {

    }



    return 0;
}