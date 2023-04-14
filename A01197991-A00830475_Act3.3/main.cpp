/**
 * Actividad 3.3 Implementación de "Knapsack problem"
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 * Este programa implementa un algoritmo de optimizacion combinatoria: el algoritmo Knapsack problem.
 * 10/04/23
 */


#include <vector>
#include <iostream>

struct Object {
    int value;
    int weight;
};

/**
 * Este algoritmo calcula la ganancia optima que se puede obtener al colocar objetos de distinto peso y valor dentro de
 * un espacio limitado. Este algoritmo se ejecuta en tiempo O(nP), con n siendo el numero de articulos y P siendo
 * el peso maximo.
 * @param objects Un vector de objetos que contiene su informacion.
 * @param maxWeight Peso maximo que soporta el contenedor.
 * @return El valor de la ganancia maxima que se pudo obtener.
 */
int knapsackSolver(std::vector<Object> const &objects, int maxWeight) {
    std::size_t objectCount = objects.size();
    std::vector<std::vector<int>> matrix(objectCount + 1, std::vector<int>(maxWeight + 1));

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
    return *((matrix.end() - 1)->end() - 1);
}

int main() {
    std::size_t n;
    std::cin >> n;
    std::vector<Object> products;
    products.reserve(n);

    Object newObject{};
    int value;
    for(int i = 0;i < n ;i++){
        std::cin >> value;
        newObject.value = value;
        products.push_back(newObject);
    }
    for(int j = 0;j < n ;j++){
        std::cin >> value;
        products.at(j).weight = value;
    }

    int maxWeight;
    std::cin >> maxWeight;
    std::cout << knapsackSolver(products, maxWeight) << std::endl;
    return 0;
}