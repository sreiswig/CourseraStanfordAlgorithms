#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

struct Vertex {
    int label;
    long long distance;
};

struct Edge {
    int from, to, weight;
};

class Graph {
    private:
        int numVertices, numEdges;
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
        std::unordered_map<int, int> distances;
    public:
        Graph(int numVertices, int numEdges, std::vector<Edge> edges) {
            this->numVertices = numVertices;
            this->numEdges = numEdges;
            this->edges = edges;
            for (int i = 1; i < numVertices + 1; i++) {
                Vertex v;
                v.label = i;
                v.distance = INT32_MAX;
                vertices.push_back(v);
            }
        }

        void djikstraShortestPath(int start) {
            // unordered_map is initialized to false
            std::set<int> unvistedVertices;
            for (auto vertex : vertices) {
                if(vertex.label == start) {
                    distances[vertex.label] = 0;
                } else {
                    distances[vertex.label] = INT32_MAX;
                    unvistedVertices.insert(vertex.label);
                }
            }
            int currentVertex = start;
            while (!unvistedVertices.empty()) {
                for (auto edge : edges) {
                    if (edge.from == currentVertex) {
                        if (distances[edge.to] > distances[edge.from] + edge.weight) {
                            distances[edge.to] = distances[edge.from] + edge.weight;
                        }
                    }
                }
                unvistedVertices.erase(currentVertex);
                int minDistance = INT32_MAX;
                for (auto vertex : unvistedVertices) {
                    if (distances[vertex] < minDistance) {
                        minDistance = distances[vertex];
                        currentVertex = vertex;
                    }
                }
            }
        }

        bool bellmanFord(int start) {
            bool negativeCycle = false;
            // set all distances to infinity
            for (auto vertex : vertices) {
                vertex.distance = INT32_MAX;
            }
            // shortest path
            for (int i = 0; i < vertices.size(); i++) {
                for (auto edge : edges) {
                    if (vertices[edge.from - 1].distance + edge.weight < vertices[edge.to - 1].distance) {
                        vertices[edge.to - 1].distance = vertices[edge.from - 1].distance + edge.weight;
                    }
                }
            }
            // negative cycle check
            for (auto edge : edges) {
                if (vertices[edge.from - 1].distance + edge.weight < vertices[edge.to - 1].distance) {
                    std::cout << "Graph contains negative cycle" << std::endl;
                    negativeCycle = true;
                    return negativeCycle;
                }
            }
            return negativeCycle;
        }

        int johnsonsAlgorithm() {
            // a new node q is added to the graph
            int extraVertex = numVertices + 1;
            Vertex q = {extraVertex, 0};
            vertices.push_back(q);
            for (int i = 1; i <= numVertices; i++) {
                Edge e = {extraVertex, i, 0};
                edges.push_back(e);
            }

            // bellman ford is used starting from q to find h(v) of a path from q to v
            // if negative cycle is found, then there is no path from q to v and algorithm is terminated
            int minShortestPath = INT32_MAX;
            if(!bellmanFord(extraVertex)) {
                // edges of the original graph are reweighted using the values from the Bellman Ford algorithm
                // w(u -> v) becomes w(u -> v) + h(u) - h(v)
                for (auto& edge : edges) {
                    edge.weight = edge.weight + vertices[edge.from - 1].distance - vertices[edge.to -1].distance;
                }
                // remove q
                vertices.pop_back();
                for (int i = 1; i <= numVertices; i++) {
                    edges.pop_back();
                }
                // dijikstra is used to find the shortest path from each vertex s to every other vertex
                for (auto vertex : vertices) {
                    std::cout << "Vertex: " << vertex.label << std::endl;
                    djikstraShortestPath(vertex.label);
                    // the distance in the original graph is computed for each distance
                    // D(u -> v) becomes D(u -> v) + h(v) - h(u)
                    for (auto distance : distances) {
                        if (distance.first == vertex.label) { continue; }
                        int currentPathDistance = distance.second + vertices[distance.first - 1].distance - vertex.distance;
                        if (currentPathDistance < minShortestPath) {
                            minShortestPath = currentPathDistance;
                        }
                    }
                }
            }
            return minShortestPath;
        }
};

std::vector<std::string> split(std::string str, char delimeter) {
    std::string token;
    std::vector<std::string> result;
    while (str.find(delimeter) != std::string::npos) {
        token = str.substr(0, str.find(delimeter));
        str.erase(0, str.find(delimeter) + 1);
        result.push_back(token);
    }
    result.push_back(str);
    return result;
}

int main() {
    std::cout << "Enter filename: ";
    std::string filename;
    std::cin >> filename;

    std::ifstream file(filename);
    std::string line;
    
    std::getline(file, line);
    char delimeter = ' ';

    int num_vertices, num_edges;
    std::vector<std::string> line_split;
    line_split = split(line, delimeter);
    num_vertices = std::stoi(line_split[0]);
    num_edges = std::stoi(line_split[1]);

    std::vector<Edge> edges;

    while (std::getline(file, line)) {
        int tail, head, weight;
        line_split = split(line, delimeter);
        tail = std::stoi(line_split[0]);
        head = std::stoi(line_split[1]);
        weight = std::stoi(line_split[2]);
        Edge edge = {head, tail, weight};
        edges.push_back(edge);
    }
    std::cout << "Vertices: " << num_vertices << std::endl;
    std::cout << "Edges: " << num_edges << std::endl;

    Graph dg = Graph(num_vertices, num_edges, edges);
    std::cout << "Min of min shortest path " << dg.johnsonsAlgorithm();
    return 0;
}