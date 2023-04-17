/**
 * Actividad 3.4 Implementación de Kruskal
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 *
 *
 * 10/04/23
 */
#include <iostream>
#include <vector>
#include <optional>
#include <queue>
#include <set>
#include <numeric>

using AdjacencyMatrix = std::vector<std::vector<int>>;

void printKruskalMinimumSpanningTree(AdjacencyMatrix const &graph) {

}

int main() {
    int graphDim;
    std::cin >> graphDim;

    AdjacencyMatrix graph(graphDim, std::vector<int>(graphDim));

    for (auto &row: graph) {
        for (auto &edge: row) {
            std::cin >> edge;
        }
    }

    printKruskalMinimumSpanningTree(graph);

    return 0;
}