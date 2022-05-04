#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

struct Edge {
    int v1;
    int v2;
    int weight;

    inline bool operator<(const Edge& rhs) const {
        return weight < rhs.weight;
    }

    inline bool operator>(const Edge& rhs) const {
        return weight > rhs.weight;
    }
};

class Vertex {
    private:
        int label;
        int cheapestCostConnection;
        bool visited;
        std::vector<Edge> adjacentEdges;

    public:
        Vertex() {
            label = 0;
            visited = false;
        }
        void setLabel(int label) {
            this->label = label;
        }
        void setVisited(bool visited) {
            this->visited = visited;
        }
        void addEdge(int v2, int weight) {
            Edge edge;
            edge.v1 = label;
            edge.v2 = v2;
            edge.weight = weight;
            adjacentEdges.push_back(edge);
        }
        void addEdge(Edge e) {
            adjacentEdges.push_back(e);
        }
        int getLabel() {
            return label;
        }
        bool getVisited() {
            return visited;
        }

        inline bool operator<(const Vertex& rhs) const {
            return cheapestCostConnection < rhs.cheapestCostConnection;
        }

        inline bool operator>(const Vertex& rhs) const {
            return cheapestCostConnection > rhs.cheapestCostConnection;
        }

        std::vector<Edge> getAdjacentEdges() {
            return adjacentEdges;
        }

        void print() {
            std::cout << "label : " << label << "| visited : " << visited << "| min cost connection : " << cheapestCostConnection << "\n";
        }
};

class Graph {
    private:
        std::unordered_map<int, Vertex> vertices;
    public:
        Graph() { }

        void addVertex(Vertex v) {
            vertices[v.getLabel()] = v;
        }

        void addEdge(int v1, int v2, int weight) {
            vertices[v1].addEdge(v2, weight);
            vertices[v2].addEdge(v1, weight);
        }

        int primMSTAlgorithm() {
            int totalCost = 0;
            std::vector<Vertex> mst;
            std::vector<Vertex> unused_set;
            std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> edges;
            for(auto v : vertices) {
                unused_set.push_back(v.second);
            }

            mst.push_back(unused_set.front());
            unused_set.erase(unused_set.begin());
            
            for (auto v : mst) {
                vertices[v.getLabel()].setVisited(true);
                for (auto edge : v.getAdjacentEdges()) {
                    edges.push(edge);
                }
            }

            while (!edges.empty()) {
                Edge edge = edges.top();
                edges.pop();
                if (vertices[edge.v1].getVisited() && vertices[edge.v2].getVisited()) {
                    continue;
                } else {
                    if (!vertices[edge.v1].getVisited()) {
                        vertices[edge.v1].print();
                        vertices[edge.v1].setVisited(true);
                        mst.push_back(vertices[edge.v1]);
                        auto vertexEdges = vertices[edge.v1].getAdjacentEdges();
                        for (auto e : vertexEdges) {
                            edges.push(e);
                        }
                        totalCost += edge.weight;
                    } else {
                        vertices[edge.v2].print();
                        vertices[edge.v2].setVisited(true);
                        mst.push_back(vertices[edge.v2]);
                        auto vertexEdges = vertices[edge.v2].getAdjacentEdges();
                        for (auto e : vertexEdges) {
                            edges.push(e);
                        }
                        totalCost += edge.weight;
                    }
                }
            }

            return totalCost;
        }

        void print() {
            for (auto v : vertices) {
                v.second.print();
            }
        }
};

int main() {
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;
    std::ifstream file(filename);
    std::string line;
    std::string delimeter = " ";
    std::getline(file, line);
    std::string nodeString = line.substr(0, line.find(delimeter));
    line.erase(0, line.find(delimeter) + 1);
    std::string edgeString = line.substr(0, line.find(delimeter));
    
    int num_vertices, num_edges;
    num_vertices = std::stoi(nodeString);
    num_edges = std::stoi(edgeString);

    Graph graph;
    for (int i = 1; i < num_vertices+1; i++) {
        Vertex v;
        v.setLabel(i);
        graph.addVertex(v);
    }

    while (std::getline(file, line)) {
        std::string v1, v2, weight;
        v1 = line.substr(0, line.find(delimeter));
        line.erase(0, line.find(delimeter) + 1);
        v2 = line.substr(0, line.find(delimeter));
        line.erase(0, line.find(delimeter) + 1);
        weight = line.substr(0, line.find(delimeter));
        graph.addEdge(std::stoi(v1), std::stoi(v2), std::stoi(weight));
    }

    std::cout << graph.primMSTAlgorithm() << "\n";
}