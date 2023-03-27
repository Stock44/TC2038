/**
 * Actividad 1.3 Implementación de la técnica de programación "backtracking" y "ramificación y poda"
 * Hiram Maximiliano Muñoz Ramirez A01197991
 * Angel Rigoberto García García A00830475
 * 02/03/2023
 */

#include <istream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <iostream>
#include <set>
#include <stack>
#include <queue>

template<typename V>
class Graph {
public:
    using vertex_descriptor = std::size_t;

    Graph::vertex_descriptor createVertex() {
        auto vertex = nextVertex;
        nextVertex++;

        store.try_emplace(vertex);

        return vertex;
    }

    void createEdge(vertex_descriptor v1, vertex_descriptor v2) {
        // undirected graph
        store.at(v1).push_back(v2);
        store.at(v2).push_back(v1);
        store.size();
    }

    template<typename T>
    void setVertexValue(vertex_descriptor vertex, T &&value) {
        vertexValues.try_emplace(vertex, std::forward<T>(value));
    }

    V &getVertexValue(vertex_descriptor vertex) {
        return vertexValues.at(vertex);
    }

    std::vector<vertex_descriptor> getVertexNeighbors(vertex_descriptor vertex) {
        return store.at(vertex);
    }


private:
    std::unordered_map<vertex_descriptor, std::vector<Graph::vertex_descriptor>> store;
    std::unordered_map<vertex_descriptor, V> vertexValues;
    Graph::vertex_descriptor nextVertex = 0;
};

using coordinate = std::pair<int, int>;
using GraphT = Graph<coordinate>;
using vertex = GraphT::vertex_descriptor;


std::tuple<GraphT, vertex, vertex> readGraphFromStream(std::istream &stream) {
    GraphT graph;

    int rows;
    int cols;

    stream >> rows >> cols;

    vertex origin;
    vertex destination;

    std::vector<std::vector<std::optional<Graph<coordinate>::vertex_descriptor>>> values(rows);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            bool walkable;
            stream >> walkable;

            if (!walkable) {
                values.at(row).emplace_back();
                continue;
            }

            auto vertex = graph.createVertex();

            if (row == 0 && col == 0) {
                origin = vertex;
            }

            if (row == rows - 1 && col == cols - 1) {
                destination = vertex;
            }

            graph.setVertexValue(vertex, coordinate(row, col));

            values.at(row).emplace_back(vertex);

            if (row > 0 && values.at(row - 1).at(col).has_value()) {
                graph.createEdge(vertex, values.at(row - 1).at(col).value());
            }

            if (col > 0 && values.at(row).at(col - 1).has_value()) {
                graph.createEdge(vertex, values.at(row).at(col - 1).value());
            }
        }
    }
    return {graph, origin, destination};
}

/**
 * Implementation of a backtracking finder for a graph with a given origin vertex and destination vertex
 * @param graph
 * @param origin
 * @param destination
 * @return vector of vertices from origin to destination
 */
std::vector<vertex> backtrackingFindRoute(GraphT &graph, vertex origin, vertex destination) {
    std::stack<vertex> verticesToVisit;
    std::unordered_map<vertex, vertex> vertexOrigins;

    verticesToVisit.push(origin);
    
    while (!verticesToVisit.empty()) {
        auto currentVertex = verticesToVisit.top();
        verticesToVisit.pop();


        auto neighbors = graph.getVertexNeighbors(currentVertex);
        if (currentVertex == destination) {
            continue;
        }
        for (auto neighbor: neighbors) {
            if (!vertexOrigins.contains(neighbor)) {
                verticesToVisit.push(neighbor);
                vertexOrigins.try_emplace(neighbor, currentVertex);
            }
        }
    }

    std::vector<vertex> path;
    auto currentVertex = destination;
    while (currentVertex != origin) {
        path.insert(path.begin(), currentVertex);
        currentVertex = vertexOrigins.at(currentVertex);
    }
    path.insert(path.begin(), currentVertex);
    return path;
}

/**
 * Implementation of a branch and bound path finder for a graph with a given origin vertex and destination vertex
 * @param graph graph to use
 * @param origin origin vertex
 * @param destination destination vertex
 * @return the vertices that go from the origin to the destination
 */
std::vector<vertex> branchAndBoundFindRoute(GraphT &graph, vertex origin, vertex destination) {
    std::queue<vertex> verticesToVisit;
    std::unordered_map<vertex, vertex> vertexOrigins;

    verticesToVisit.push(origin);
    while (!verticesToVisit.empty()) {
        auto currentVertex = verticesToVisit.front();
        verticesToVisit.pop();

        if (currentVertex == destination) {
            continue;
        }

        for (auto neighbor: graph.getVertexNeighbors(currentVertex)) {
            if (!vertexOrigins.contains(neighbor)) {
                verticesToVisit.push(neighbor);
                vertexOrigins.emplace(neighbor, currentVertex);
            }
        }
    }

    std::vector<vertex> route;
    auto currentVertex = destination;
    while (currentVertex != origin) {
        route.insert(std::begin(route), currentVertex);
        currentVertex = vertexOrigins.at(currentVertex);
    }
    route.insert(std::begin(route), currentVertex);

    return route;
}

/**
 * Outputs the maze graph's representation to a output stream.
 * @param graph
 * @param route
 * @param stream
 */
void outputRouteToStream(GraphT &graph, std::vector<vertex> &route, std::ostream &stream) {
    auto [rows, cols] = graph.getVertexValue(*(std::end(route) - 1));

    std::vector<std::vector<int>> grid(rows + 1);
    std::fill(grid.begin(), grid.end(), std::vector<int>(cols + 1));
    for (auto &row: grid) {
        std::fill(row.begin(), row.end(), 0);
    }

    for (auto vertex: route) {
        auto [row, col] = graph.getVertexValue(vertex);

        grid.at(row).at(col) = 1;
    }

    for (auto const &row: grid) {
        for (auto num: row) {
            stream << num << " ";
        }
        stream << "\n";
    }
}

int main() {
    auto [graph, originVertex, destinationVertex] = readGraphFromStream(std::cin);
    auto branchAndBoundRoute = branchAndBoundFindRoute(graph, originVertex, destinationVertex);
    auto backtrackingRoute = backtrackingFindRoute(graph, originVertex, destinationVertex);

    outputRouteToStream(graph, backtrackingRoute, std::cout);
    std::cout << "\n";

    outputRouteToStream(graph, branchAndBoundRoute, std::cout);

    std::cout.flush();
    return 0;
}


