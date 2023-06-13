//
// Created by hiram on 6/13/23.
//
#include <vector>
#include <optional>
#include <unordered_map>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <numeric>
#include <stack>

template<typename V, typename E>
class Graph {
public:
    using VertexDescriptor = std::size_t;

    [[nodiscard]] std::vector<VertexDescriptor> vertices() const &{
        auto _vertices = std::vector<VertexDescriptor>(store.size());

        std::iota(_vertices.begin(), _vertices.end(), 0);

        return _vertices;
    }

    template<typename T>
    VertexDescriptor addVertex(T &&value) {
        VertexDescriptor vertex = store.size();
        for (auto &row: store) {
            row.emplace_back();
        }
        store.emplace_back(vertex + 1, E());
        vertexProperties.emplace(vertex, std::forward<T>(value));
        return vertex;
    }

    template<typename T>
    void addEdge(VertexDescriptor v1, VertexDescriptor v2, T &&value) {
        store.at(v1).at(v2) = std::forward<T>(value);
    }

    std::optional<E> const &operator[](std::pair<VertexDescriptor, VertexDescriptor> edge) const {
        return store.at(edge.first).at(edge.second);
    }

    V const &operator[](VertexDescriptor const &vertex) const {
        return vertexProperties.at(vertex);
    }

    std::optional<E> &operator[](std::pair<VertexDescriptor, VertexDescriptor> edge) {
        return store.at(edge.first).at(edge.second);
    }

    V &operator[](VertexDescriptor const &vertex) {
        return vertexProperties.at(vertex);
    }

    [[nodiscard]] std::vector<VertexDescriptor> neighbors(VertexDescriptor vertex) const {
        std::vector<VertexDescriptor> neighbors;
        auto const &possibleNeighbors = store.at(vertex);

        for (VertexDescriptor v = 0; v < possibleNeighbors.size(); v++) {
            if (v == vertex) continue;
            if (possibleNeighbors.at(v).has_value()) {
                neighbors.push_back(v);
            }
        }

        return neighbors;
    }

private:
    std::vector<std::vector<std::optional<E>>> store;
    std::unordered_map<VertexDescriptor, V> vertexProperties;
};

struct Point {
    double x;
    double y;
};

double distance(Point p1, Point p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

template<typename V, typename E, typename H>
std::vector<typename Graph<V, E>::VertexDescriptor> aStar(Graph<V, E> const &graph, typename Graph<V, E>::VertexDescriptor start,
           typename Graph<V, E>::VertexDescriptor end, H &&heuristic) {
    using Vertex = Graph<V, E>::VertexDescriptor;

    std::unordered_map<Vertex, Vertex> cameFrom;

    std::unordered_map<Vertex, std::optional<E>> pathCosts;
    std::unordered_map<Vertex, std::optional<E>> fullCosts;

    for (auto vertex: graph.vertices()) {
        pathCosts.emplace(vertex, std::optional<E>());
        fullCosts.emplace(vertex, std::optional<E>());
    }

    pathCosts.at(start) = E();
    fullCosts.at(start) = heuristic(start);

    auto comp = [&fullCosts](Vertex const &v1, Vertex const &v2) {
        if (!fullCosts.at(v1).has_value()) return false;
        if (!fullCosts.at(v2).has_value()) return true;
        return fullCosts.at(v1).value() < fullCosts.at(v2).value();
    };

    std::priority_queue<Vertex, std::vector<Vertex>, decltype(comp)> openQueue(comp);
    std::unordered_set<Vertex> openSet;

    openQueue.push(start);
    openSet.emplace(start);


    while (!openQueue.empty()) {
        auto current = openQueue.top();
        if (current == end) {
            break;
        }
        openSet.erase(current);
        openQueue.pop();

        for (auto neighbor: graph.neighbors(current)) {
            auto pathCost = pathCosts.at(current).value() + graph[std::make_pair(current, neighbor)].value();

            if (!pathCosts.at(neighbor).has_value()) {
                cameFrom.emplace(neighbor, current);
            } else if (pathCost < pathCosts.at(neighbor).value()) {
                cameFrom.at(neighbor) = current;
            } else {
                continue;
            }

            pathCosts.at(neighbor) = pathCost;
            fullCosts.at(neighbor) = pathCost + heuristic(neighbor);

            if (!openSet.contains(neighbor)) {
                openQueue.push(neighbor);
                openSet.emplace(neighbor);
            }
        }
    }

    std::stack<Vertex> pathStack;
    pathStack.emplace(openQueue.top());
    while(pathStack.top() != start) {
        auto origin = cameFrom.at(pathStack.top());
        pathStack.emplace(origin);
    }

    std::vector<Vertex> path;
    path.reserve(pathStack.size());
    while(!pathStack.empty()) {
        path.push_back(pathStack.top());
        pathStack.pop();
    }

    return path;
}

int main() {
    Graph<Point, double> graph;

    std::size_t n;
    std::cin >> n;

    std::vector<decltype(graph)::VertexDescriptor> vertices;
    for (std::size_t i = 0; i < n; i++) {
        vertices.push_back(graph.addVertex(Point()));
    }

    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j < n; j++) {
            double value;
            std::cin >> value;
            if (value > 0) {
                graph.addEdge(vertices.at(i), vertices.at(j), value);
            }
        }
    }

    for (auto vertex: vertices) {
        std::cin >> graph[vertex].x;
        std::cin >> graph[vertex].y;
    }

    auto start = *vertices.begin();
    auto end = *(vertices.end() - 1);
    auto endPos = graph[end];

    auto path = aStar(graph, start, end, [&graph, &endPos](auto vertex) {
        return distance(graph[vertex], endPos);
    });

    double distance = 0;
    for (auto it = path.begin() + 1; it != path.end(); it++) {
        std::cout << *(it - 1) << " to " << *it << " | cost: " << graph[std::make_pair(*(it - 1), *it)].value() << "\n";
        distance += graph[std::make_pair(*(it - 1), *it)].value();
    }

    std::cout << distance << "\n";

    std::cout.flush();
    return 0;
}