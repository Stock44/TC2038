/**
 * Actividad 3.5 Implementación de Gilbert and Moore
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 *
 * This file contains an implementation of the Gilbert and Moore algorithm for finding the optimum binary search tree
 * for a given input of node search probabilities.
 *
 * Compiled on Arch Linux (Linux 6.2.10) with the gcc compiler toolchain version 12.2.1
 * g++ -o out -std=c++20 main.cpp
 * ./out
 *
 * 17/04/23
 */

#include <vector>
#include <complex>
#include <iostream>

struct BSTNode {
    int k;
    int i;
    int j;
};

/**
 * Recursively prints the children of all nodes in the optimum BST in O(n), with n being the number of nodes in the tree.
 * @param node
 * @param kValues
 */
void printChildren(BSTNode node, std::vector<std::vector<BSTNode>> const &kValues) {
    auto &leftChild = kValues[node.i][node.k - 1];
    auto &rightChild = kValues[node.k + 1][node.j];
    if (leftChild.k != 0) {
        std::cout << "nodo " << node.k << " es el padre de su hijo izquierdo nodo " << leftChild.k << "\n";
        printChildren(leftChild, kValues);
    }
    if (rightChild.k != 0) {
        std::cout << "nodo " << node.k << " es el padre de su hijo derecho nodo " << rightChild.k << "\n";
        printChildren(rightChild, kValues);
    }

}

/**
 * Finds the optimum binary search tree for a vector of probabilities in O(n^3) time.
 * @param probabilities
 * @return
 */
float optimumBST(std::vector<float> probabilities) {
    std::size_t nodeCount = probabilities.size();

    std::vector<float> accumProbabilities(nodeCount + 1);
    for (std::size_t i = 1; i <= nodeCount; i++) {
        for (std::size_t j = 0; j < i; j++) {
            accumProbabilities[i] += probabilities[j];
        }
    }


    std::vector<std::vector<float>> subtreeMatrix(nodeCount + 2, std::vector<float>(nodeCount + 2, 0));
    std::vector<std::vector<BSTNode>> kValues(nodeCount + 2, std::vector<BSTNode>(nodeCount + 2));

    for (int i = 1; i <= nodeCount; i++) {
        subtreeMatrix[i][i] = probabilities[i - 1];
        kValues[i][i] = {i, i, i};
    }
    for (int diag = 1; diag < nodeCount; diag++) {
        for (int i = 1; i <= nodeCount - diag; i++) {
            int j = i + diag;
            float min = subtreeMatrix[i][i - 1] + subtreeMatrix[i + 1][j];
            BSTNode minK = {i, i, j};
            for (int k = i + 1; k <= j; k++) {
                if (subtreeMatrix[i][k - 1] + subtreeMatrix[k + 1][j] < min) {
                    min = subtreeMatrix[i][k - 1] + subtreeMatrix[k + 1][j];
                    minK.k = k;
                }
            }

            kValues[i][j] = minK;
            subtreeMatrix[i][j] = min + accumProbabilities[j] - accumProbabilities[i - 1];
        }
    }

    printChildren(kValues[1][nodeCount], kValues);

    return subtreeMatrix[1][nodeCount];
}


int main() {
    std::vector<float> p = {0.06, 0.11, 0.14, 0.07, 0.34, 0.12, 0.16};
    std::cout << optimumBST(p) << std::endl;

    return 0;
}