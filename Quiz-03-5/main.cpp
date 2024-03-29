#include <vector>
#include <limits>
#include <iostream>

void floyd(std::vector<std::vector<int>> &adjacencyMatrix) {
    auto n = adjacencyMatrix.size();

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adjacencyMatrix[i][j] != std::numeric_limits<int>::max() &&
                    adjacencyMatrix[k][j] != std::numeric_limits<int>::max() &&
                    adjacencyMatrix[i][k] + adjacencyMatrix[k][j] < adjacencyMatrix[i][j]) {
                    adjacencyMatrix[i][j] = adjacencyMatrix[i][k] + adjacencyMatrix[k][j];
                }
            }
        }
    }
}

int main() {
    std::vector<std::vector<int>> graph = {
            {0, 769, 954, 952, 945, 104, 934, 347, 164, 365},
            {769, 0, 718, 832, 162, 723, 228, 758, 424, 606},
            {954, 718, 0, 488, 834, 197, 913, 196, 585, 895},
            {952, 832, 488, 0, 475, 100, 658, 842, 229, 646},
            {945, 162, 834, 475, 0, 119, 185, 193, 313, 175},
            {104, 723, 197, 100, 119, 0, 537, 647, 916, 401},
            {934, 228, 913, 658, 185, 537, 0, 429, 112, 207},
            {347, 758, 196, 842, 193, 647, 429, 0, 249, 259},
            {164, 424, 585, 229, 313, 916, 112, 249, 0, 694},
            {365, 606, 895, 769, 0, 718, 832, 162, 723, 228, 758, 424, 606},
            {954, 718, 0, 488, 834, 197, 913, 196, 585, 895},
            {952, 832, 488, 0, 475, 100, 658, 842, 229, 646},
            {945, 162, 834, 475, 0, 119, 185, 193, 313, 175},
            {104, 723, 197, 100, 119, 0, 537, 647, 916, 401},
            {934, 228, 913, 658, 185, 537, 0, 429, 112, 207},
            {347, 758, 196, 842, 193, 647, 429, 0, 249, 259},
            {164, 424, 585, 229, 313, 916, 112, 249, 0, 694},
           646, 175, 401, 207, 259, 694, 0}
    };

    floyd(graph);

    std::cout << graph[3][2];

    return 0;
}