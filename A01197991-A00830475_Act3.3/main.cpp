#include <vector>

struct Object {
    int value;
    int weight;
};

int knapsackSolver(std::vector<Object> const &objects, int maxWeight) {
    std::size_t objectCount = objects.size();
    std::vector<std::vector<int>> matrix(objectCount + 1, {maxWeight + 1});

    for (int i = 1; i <= objectCount; i++) {
        for (int j = 1; j <= maxWeight; j++) {
            if (objects.at(i - 1).weight > j) {
                matrix.at(i).at(j) = matrix.at(i - 1).at(j);
            } else {
                matrix.at(i).at(j) = std::max(matrix.at(i - 1).at(j), objects.at(i - 1).value +
                                                                      matrix.at(i - 1).at(
                                                                              j - objects.at(i - 1).weight));
            }
        }
    }
    return *matrix.end()->end();
}

int main() {
    return 0;
}