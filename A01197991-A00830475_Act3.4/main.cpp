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

struct edge
{
    int start;
    int longitude;
    int end;
};
using AdjacencyMatrix = std::vector<std::vector<int>>;

void printKruskalMinimumSpanningTree(AdjacencyMatrix const &graph) {
    int total = 0, iter = 2;
    std::vector<edge> edges;
    std::vector<std::vector<int>> vertex;
    size_t n = graph.size();

    for (int k = 1; k <= n; k++){
        vertex.at(k-1).emplace_back(k);
    }
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            edge newEdge{
                i,
                graph.at(i).at(j),
                j
            };
            edges.emplace_back(newEdge);
        }
    }
    //std::sort(edges.begin(),edges.end()); falta ordenar el vector

    //Primer arista agregada
    std::cout << edges.at(0).longitude << std::endl;
    total += edges.at(0).longitude;

    vertex.at(edges.at(0).start).emplace_back(edges.at(0).end);

    //Segunda arista agregada
    std::cout << edges.at(1).longitude << std::endl;
    total += edges.at(1).longitude;

    for(auto &data : vertex.at(edges.at(1).end)){
        //if(){  comprobar si el int no esta en vector de inicio.
            vertex.at(edges.at(1).start).emplace_back(data);
        //}
    }
    vertex.at(edges.at(1).start).emplace_back(edges.at(1).end);

    //Proceso para comprobar que no hay bucle en las siguientes aristas
    size_t edgeSize = edges.size();
    while(vertex.at(0).size() != n){
        edges.at(iter);

        //if(vertex.at(edges.at(iter).end) == vertex.at(edges.at(iter).end)){ Comprobar si el inicio y final de edges esta dentro de el vector inicial.

        }
        for(auto &data : vertex.at(edges.at(iter).end)){
            //if(){ // comprobar si el int no esta en vector de inicio.
                vertex.at(edges.at(iter).start).emplace_back(data);
            //}
        //}
        iter++;
    }

    std::cout << total << std::endl;
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