/**
 * Actividad 3.5 Implementación de Gilbert and Moore
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This file contains an implementation of a Disjoint Set data structure, as well as of Kruskal's algorithm
 * for finding the minimum spanning forest / tree in an undirected graph represented as an adjacency matrix.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 12.2.1
 * g++ -o out -std=c++20 main.cpp
 * ./out
 *
 * 17/04/23
 */

#include <vector>
#include <complex>
#include <optional>
#include <functional>
#include <iostream>
#include <algorithm>
#include <memory>

struct BinaryTreeNode {
    BinaryTreeNode *parent;
    std::unique_ptr<BinaryTreeNode> leftChild;
    std::unique_ptr<BinaryTreeNode> rightChild;
};

float optimumBST(std::vector<float> probabilities) {
    std::size_t nodeCount = probabilities.size();

    std::vector<float> accumProbabilities(nodeCount + 1);
    for (std::size_t i = 1; i <= nodeCount; i++) {
        for (std::size_t j = 0; j < i; j++) {
            accumProbabilities[i] += probabilities[j];
        }
    }

    std::vector<std::vector<float>> subtreeMatrix(nodeCount + 2, std::vector<float>(nodeCount + 2, 0));
    std::vector<std::vector<std::size_t>> reconstructionMatrix(nodeCount + 2, std::vector<std::size_t>(nodeCount + 2, 0));

    for (int i = 1; i <= nodeCount; i++) {
        subtreeMatrix[i][i] = probabilities[i - 1];
    }
    for (int diag = 1; diag < nodeCount; diag++) {
        for (int i = 1; i <= nodeCount - diag; i++) {
            int j = i + diag;
            float min = subtreeMatrix[i][i - 1] + subtreeMatrix[i + 1][j];
            int minK = i;
            for (int k = i + 1; k <= j; k++) {
                if (subtreeMatrix[i][k - 1] + subtreeMatrix[k + 1][j] < min) {
                    min = subtreeMatrix[i][k - 1] + subtreeMatrix[k + 1][j];
                    minK = k;
                }
            }
            reconstructionMatrix[i][j] = minK;
            subtreeMatrix[i][j] = min + accumProbabilities[j] - accumProbabilities[i - 1];
        }
    }

    return subtreeMatrix[1][nodeCount];
}

int main() {
//    std::size_t nodeCount;
//    std::cin >> nodeCount;
//    std::vector<float> probabilities(nodeCount);
//    for (std::size_t i = 0; i < nodeCount; i++) {
//        std::cin >> probabilities.at(i);
//    }

    std::vector<float> p = {0.06, 0.11, 0.14, 0.07, 0.34, 0.12, 0.16};
    std::cout << optimumBST(p) << std::endl;
//    auto [bst, prob] = optimumBST(probabilities);
//    bst.visitInorder([](BinarySearchTree<std::size_t>::Node node) {
//        std::cout << "node " << node.value() << " with parent "
//                  << (node.parent() ? std::to_string(node.parent().value().value()) : "none") << "\n";
//    });
//    std::cout << prob << std::endl;
    return 0;
}