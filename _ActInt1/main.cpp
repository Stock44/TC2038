/**
 * Actividad Integradora
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 * 30/03/2023
 */
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <string>

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

/**
 * Finds the longest palindrome within a string in O(n) time
 * @param S
 * @return
 */
std::pair<int, int> findLongestPalindrome(std::string const &S) {
    std::pair<int, int> res; // resul tado (inicio, longi tud)
    if (S.length() == 0) // S es nulo
        return res;
    std::string T;
    for (char c: S) {
        T += "|";
        T += c;
    }
    T += "|";

    int N = T.length();
// longi tud y centro del mäximo palindromo encontrado
    int maxLong = 1, maxCentro = 1; // Hasta ahora posicåön I
    int L[1000]; //int L[N];
    int C = 1;
    int Li = 0, Ri = 0;
    bool expansion = false; // true si requiera expansidn
    L[0] = 0;
    L[1] = 1;
    for (Ri = 2; Ri < N; Ri++) {
        expansion = false;
        Li = C - (Ri - C);
        if ((C + L[C]) - Ri >= 0) {
            if (L[Li] < (C + L[C]) - Ri) // caso 1
                L[Ri] = L[Li];
            else if (L[Li] == (C + L[C]) - Ri && (C + L[C]) == N - 1) // Caso 2
                L[Ri] = L[Li];
            else if (L[Li] == (C + L[C]) - Ri && (C + L[C]) < N - 1) { // Caso 3
                L[Ri] = L[Li];
                expansion = true; // requiere expansi6n
            } else if (L[Li] > (C + L[C]) - Ri) { // Case 4
                L[Ri] = (C + L[C]) - Ri;
                expansion = true; // requiere expansidn
            }
        } else {
            L[Ri] = 0;
            expansion = true; // requiere expansi6n
        }
        if (expansion) // hacer la expansidn hasta donde se pueda
            while ((Ri + L[Ri]) < N && (Ri - L[Ri]) > 0
                   && T[Ri + L[Ri] + 1] == T[Ri - L[Ri] - 1])
                L[Ri]++;
        if (Ri + L[Ri] > (C + L[C]))
// si el nuevo palindromo se expande rnés allé de C
            C = Ri;
        if (L[Ri] > maxLong) {
// Guardar longitud y centro del palíndromo más grande,
// hasta ahora
            maxLong = L[Ri];
            maxCentro = Ri;
        }
    }
// obtener inicio y longi tud del máximo palíndromo encontrado
// recordando que la longitud de T es el doble de la de S
    res.first = (maxCentro - maxLong) / 2; // inicio en S
    res.second = maxLong; // longitud en S
    return res;
}

/**
 * Reads all of a files contents and stores them in a string, discarding newlines
 * @param file
 * @return
 */
std::string readWholeFile(std::ifstream &file) {
    std::string fileContents;

    for (std::string line; std::getline(file, line);) {
        fileContents.append(line);
    }

    return fileContents;
}

/**
 * Finds the longest common substring from two strings, in O(m * n) time, where m and n are the lengths
 * of both input strings, respectively.
 * @param s1
 * @param s2
 * @return
 */
std::pair<int, int> longestCommonSubstring(std::string s1, std::string s2) {
    int len1 = s1.length();
    int len2 = s2.length();
    int dp[2][len2 + 1];
    int curr = 0;
    int res = 0;
    int end = 0;
    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            if (i == 0 || j == 0)
                dp[curr][j] = 0;
            else if (s1[i - 1] == s2[j - 1]) {
                dp[curr][j] = dp[1 - curr][j - 1] + 1;
                if (res < dp[curr][j]) {
                    res = dp[curr][j];
                    end = i - 1;
                }
            } else {
                dp[curr][j] = 0;
            }
        }
        curr = 1 - curr;
    }
    return {end - res + 1, res};
}

int main() {
    std::vector<std::ifstream> transmissionFiles;
    std::vector<std::string> transmissions;
    std::vector<std::vector<std::string>> suffixArrays;
    std::vector<std::ifstream> maliciousCodeFiles;
    std::vector<std::string> maliciousCodes;
    std::vector<int> idk;

    transmissionFiles.emplace_back("transmission1.txt");
    transmissionFiles.emplace_back("transmission2.txt");

    maliciousCodeFiles.emplace_back("mcode1.txt");
    maliciousCodeFiles.emplace_back("mcode2.txt");
    maliciousCodeFiles.emplace_back("mcode3.txt");

    for (auto &file: transmissionFiles) {
        std::string const &transmission = transmissions.emplace_back(readWholeFile(file));
        suffixArrays.emplace_back(generateSuffixArray(transmission));
    }

    std::transform(maliciousCodeFiles.begin(), maliciousCodeFiles.end(), std::back_inserter(maliciousCodes),
                   readWholeFile);

    auto transmissionIt = transmissions.begin();
    for (auto const &suffixArray: suffixArrays) {
        for (auto const &maliciousCode: maliciousCodes) {
            auto suffixIt = std::lower_bound(suffixArray.begin(), suffixArray.end(), maliciousCode);
            auto const &suffix = *suffixIt;
            auto [mCodeCharIt, suffixCharIt] = std::mismatch(maliciousCode.begin(), maliciousCode.end(),
                                                             suffix.begin());

            bool found = mCodeCharIt == maliciousCode.end() && suffixCharIt != maliciousCode.end();
            if (!found) std::cout << "false\n";
            else {
                std::cout << "true " << transmissionIt->length() - suffix.length() << "\n";
            }
        }

        transmissionIt++;
    }

    for (auto const &transmission: transmissions) {
        auto [position, length] = findLongestPalindrome(transmission);
        std::cout << position << " " << position + length << "\n";
    }

    auto [position, count] = longestCommonSubstring(transmissions[0], transmissions[1]);

    std::cout << position << " " << position + count;

    return 0;
}