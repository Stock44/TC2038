/**
 * Actividad 2.1 Implementación de "Hash String"
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 * This file implements a hash algorithm for any ASCII string.
 * 02/03/2023
 */

#include <string>
#include <iostream>
#include <fstream>
#include <vector>


/**
 * This obtains the hexadecimal representation for a decimal number n
 * @param n
 * @return
 */
std::string hexadecimalRepresentation(int n) {
    std::string output;
    while (n > 0) {
        switch (n % 16) {
            case 0:
                output += '0';
                break;
            case 1:
                output += '1';
                break;
            case 2:
                output += '2';
                break;
            case 3:
                output += '3';
                break;
            case 4:
                output += '4';
                break;
            case 5:
                output += '5';
                break;
            case 6:
                output += '6';
                break;
            case 7:
                output += '7';
                break;
            case 8:
                output += '8';
                break;
            case 9:
                output += '9';
                break;
            case 10:
                output += 'A';
                break;
            case 11:
                output += 'B';
                break;
            case 12:
                output += 'C';
                break;
            case 13:
                output += 'D';
                break;
            case 14:
                output += 'E';
                break;
            case 15:
                output += 'F';
                break;
        }
        n = n / 16;
    }
    return output;
}


/**
 * This function obtains the hash for a given input string and a value n
 * @param inputString
 * @param n
 * @return
 */
std::string hashString(std::string_view inputString, int n) {
    std::vector<std::vector<char>> charTable(n);
    int col = 0;
    for (auto strChar: inputString) {
        charTable.at(col).push_back(strChar);
        col++;
        if (col >= n) {
            col = 0;
        }
    }
    for (; col < n; col++) {
        charTable.at(col).push_back('0');
    }

    std::string hash;
    hash.reserve(2 * n);

    for (auto const &col: charTable) {
        int sum = 0;
        for (auto value: col) {
            sum += value;
        }
        sum = sum % 256;
        hash += hexadecimalRepresentation(sum);
    }


    return hash;
}

int main() {
    std::string filename;
    int n;
    std::cin >> filename;
    std::cin >> n;
    std::ifstream file(filename);
    std::istreambuf_iterator<char> eos;
    std::string fileString(std::istreambuf_iterator<char>(file), eos);
    auto hash = hashString(fileString, n);
    std::cout << hash << std::endl;
    return 0;
}

