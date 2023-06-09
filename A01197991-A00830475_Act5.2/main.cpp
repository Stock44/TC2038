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
#include <cmath>

int BinarySearch(std::vector<int> list, int n) {
    size_t start = 0;
    size_t end = list.size() - 1;
    size_t middle = (start + end)/2;

    while(list.at(middle) != n){
        if(list.at(middle) > n){
            end = middle + 1;
            middle = (start + end)/2;
        }
        else if(list.at(middle) < n){
            start = middle - 1;
            middle = (start + end)/2;
        }
        if(middle == start || middle == end){
            break;
        }
    }
    if(list.at(middle) == n){
        return middle;
    }
    return -1;
}

std::pair<std::vector<int>,std::vector<int>> backtracking(std::vector<int> values, int objective){
    std::vector<int> accepted;
    std::vector<int> denied;
    int result;
    for(int i = 0; i < values.size(); i++){
        result = 0;
        for(int j = i; j < values.size(); j++){
            result += values.at(j);
            if(result == objective){
                accepted.emplace_back(result);
            }
            else denied.emplace_back(result);

        }
    }
    return {accepted, denied};
}



auto meetInMiddle(const std::vector<int>& values, int objective){
    std::vector<int> First;
    std::vector<int> Second;
    int search;
    int cont = 0;
    for(auto value:values){
        if(cont >= values.size()/2){
            First.emplace_back(value);
        }
        else{
            Second.emplace_back(value);
        }
        cont++;
    }
    auto [accepted, denied] = backtracking(First, objective);
    auto [acceptedSecond, deniedSecond] = backtracking(Second, objective);

    std::sort(deniedSecond.begin(), deniedSecond.end());

    for (auto deny:denied){
        search = objective - deny;
        auto index = BinarySearch(deniedSecond, search);
        auto token = deniedSecond.at(index);
        if(search == token){

            while(true){
                if(deniedSecond.at(index - 1) == token && index != 0){
                    index --;
                }
                else break;
            }
            while (token == deniedSecond.at(index)){
                index++;
                accepted.emplace_back(token);
            }
        }
    }
    for(auto accept:acceptedSecond){
        accepted.emplace_back(accept);
    }
    return accepted;
}



int main() {
    std::size_t n;
    std::vector<int> values;
    int value;
    int objective;
    std::cin>>n;
    for(int i = 0; i < n; i++){
        std::cin>>value;
        values.emplace_back(value);
    }
    std::cin>>objective;

    auto accepted = meetInMiddle(values, objective);

    std::cout << accepted.size();
    return 0;
}