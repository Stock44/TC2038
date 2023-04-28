/**
 * Actividad 3.6 Implementación de "Graph coloring"
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * Este programa contiene una implementación del algorithmo de Welsh-Powell para un grafo no dirigido
 * representado como una matriz de adjacencia.
 *
 * 27/04/23
 */
#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <algorithm>

using AdjacencyMatrix = std::vector<std::vector<int>>;

/**
 * Gets the degree of a particular node of an adjacency matrix in O(V) time, with V being the number
 * of nodes in the graph, as the algorithm must traverse all of the nodes in the graph to find its
 * adjacencies.
 * @param node
 * @param graph
 * @return
 */
int getNodeDegree(std::size_t node, AdjacencyMatrix const &graph) {
    auto &row = graph.at(node);
    int degree = 1;
    for (std::size_t neighbor = 0; neighbor < graph.size(); neighbor++) {
        if (neighbor == node) continue;
        if (row.at(neighbor) == 1) degree++;
    }
    return degree;
}

/**
 * This algorithm colors all nodes in a given undirected graph represented by an adjacency matrix in O(V^2) time.
 * @param graph
 * @return
 */
std::vector<int> colorGraph(AdjacencyMatrix const &graph) {
    auto n = graph.size();
    std::vector<std::size_t> nodesByDegree(n);
    std::vector<std::size_t> nodeDegrees(n);
    for (std::size_t node = 0; node < n; node++) {
        nodesByDegree.at(node) = node;
        nodeDegrees.at(node) = getNodeDegree(node, graph);
    }

    std::ranges::sort(nodesByDegree, [&nodeDegrees](std::size_t node1, std::size_t node2) {
        return nodeDegrees.at(node1) < nodeDegrees.at(node2);
    });

    int colNum = 0;
    std::vector<int> colored(n, 0);
    for (int it = 0; it < n; it++) {
        auto i = nodesByDegree[it];
        if (!colored[i]) {
            colored[i] = ++colNum;
            std::unordered_set<std::size_t> conj;
            for (int it2 = it + 1; it2 < n; it2++) {
                auto j = nodesByDegree[it2];
                if (!colored[j] && !graph[i][j]) {
                    int cia = 0;
                    for (auto ia: conj) {
                        if (graph[ia][j]) {
                            cia++;
                        }
                    }
                    if (!cia) {
                        colored[j] = colNum;
                        conj.insert(j);
                    }
                }
            }
        }
    }
    return colored;
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

    auto colors = colorGraph(graph);

    for (std::size_t node = 0; node < graph.size(); node++) {
        std::cout << "Node: " << node << ", Assigned Color: " << colors.at(node) << "\n";
    }

    std::cout.flush();

    return 0;
}