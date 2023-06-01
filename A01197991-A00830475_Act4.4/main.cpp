/**
 * Actividad 4.4 Implementación Randomized Binary Search
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This file implements the Randomized Binary Search Algorithm to determine if an integer
 * is in a set of integers.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 12.2.1
 * g++ -o out -std=c++20 main.cpp
 * ./out
 *
 * 31/05/23
 */
#include <iostream>
#include <vector>
#include <cstdlib>


/**
 * This algorithm takes a list of integers and creates a random index to compare if its the searched number, if not
 * then the range of index to search changes, this algorithm run in O(n) in the worst case.
 * @param list
 * @param n
 * @return index of n
 */
int randomizedBinarySearch(std::vector<int> list, int n) {
    int start = 0;
    int end = list.size();
    int random = rand() % list.size();

    while(list.at(random) != n){
        if(list.at(random) > n){
            end = random + 1;
            random = start + rand() % (end - start);
        }
         else if(list.at(random) < n){
            start = random - 1;
            random = start + rand() % (end - start);
        }
        if(random == start || random == end){
            break;
        }
    }
    if(list.at(random) == n){
        return random;
    }
    return -1;
}

int main(){
    int toSearch, data, n;
    std::vector<int> integers;
    std::cin >> toSearch;
    std::cin >> n;
    for(int i = 0;i < n;i++){
        std::cin >> data;
        integers.emplace_back(data);
    }
    std::cout << randomizedBinarySearch(integers, toSearch) << std::endl;

    return 0;
}