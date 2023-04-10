/**
 * Actividad 1.1 Implementación de la técnica de programación "divide y vencerás"
 * This file implements a generic, recursive implementation of the merge sort algorithm.
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 * 20/02/2023
 */
#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

void modify(std::string &input) {}

/**
 * printRange streams to std::cout a range of values
 * @tparam R type of range to sort
 * @param range reference to the range to print
 */
template<std::ranges::range R>
void printRange(R const &range) {
    for (auto value: range) {
        std::cout << value << "\n";
    }
    std::cout << std::endl;
    std::string hola = "3";
    try {
        int num = std::stoi(hola);
        std::cin >> num;
    } catch(std::invalid_argument &e) {
        std::cout << "oh no" << std::endl;
    }
}


/**
 * mergeSort recursively sorts a range using the Merge Sort algorithm. It runs with O(n log n) time complexity.
 * @tparam R Type of range to sort, that implements the random_access_range concept
 * @param range Reference to the range to sort
 */
template<std::ranges::random_access_range R>
void mergeSort(R &range) {
    namespace ranges = std::ranges;
    using ranges::begin;
    using ranges::end;
    using ranges::advance;

    auto size = ranges::size(range);

    // If range only has one element or less, it is already sorted
    if (size <= 1) return;

    // Split the range in two halves
    auto firstHalf = ranges::subrange(begin(range), begin(range) + size / 2);
    auto secondHalf = ranges::subrange(begin(range) + size / 2, end(range));

    // Sort each half recursively
    mergeSort(firstHalf);
    mergeSort(secondHalf);

    // Merge the two sorted halves using a temporary vector
    auto firstIt = begin(firstHalf);
    auto secondIt = begin(secondHalf);

    std::vector<std::ranges::range_value_t<R>> temp;
    temp.reserve(size);

    while (firstIt != end(firstHalf) && secondIt != end(secondHalf)) {
        if (*firstIt > *secondIt) {
            temp.push_back(*secondIt);
            secondIt++;
        } else {
            temp.push_back(*firstIt);
            firstIt++;
        }
    }

    while (firstIt != end(firstHalf)) {
        temp.push_back(*firstIt);
        firstIt++;
    }
    while (secondIt != end(secondHalf)) {
        temp.push_back(*secondIt);
        secondIt++;
    }

    std::ranges::swap_ranges(range, temp);
}


int main() {
    int count;
    double num;
    std::cin >> count;

    std::vector<double> numbers;
    numbers.reserve(count);

    for (int i = 0; i < count; i++) {
        std::cin >> num;
        numbers.push_back(num);
    }

    mergeSort(numbers);
    printRange(numbers);

    return 0;
}
