/**
 * Actividad 5.2 Implementación encontrarse en el medio (meet in the middle)
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This file implements a basic backtracking algorithm that uses bit masks to improve time complexity.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 12.2.1
 * g++ -o out -std=c++20 main.cpp
 * ./out
 *
 * 08/06/23
 */
#include <iostream>
#include <vector>

std::pair<std::vector<int>,std::vector<int>> backtracking(std::vector<int> values, int objective){
    std::vector<int> accepted;
    std::vector<int> denied;
    for(auto value:values){
        for(auto set:values){
            if(value == set){
                if(set == objective){
                    accepted.emplace_back(set);
                }
                else{
                    denied.emplace_back(set);
                }
            }
        }
    }
}




int main() {
    std::size_t n;
    std::vector<int> values;
    int value;
    int objective;
    for(int i = 0; i < n; i++){
        std::cin>>value;
        values.emplace_back(value);
    }
    std::cin>>objective;


    return 0;
}