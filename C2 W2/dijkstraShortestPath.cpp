#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

struct Edge {
    int to;
    int weight;
};

class Vertex {
    private:
        int label;
        int shortestPathDistance;
        std::vector<Edge> adjacentEdges;
    public:
        Vertex() { 
            label = 0;
            shortestPathDistance = INT_MAX;
        }

        // No duplicate edges
        void addEdge(int to, int weight) {
            Edge edge;
            edge.to = to;
            edge.weight = weight;
            adjacentEdges.push_back(edge);
        }

        void setLabel(int label) {
            this->label = label;
        }
        void setPathDistance(int distance) {
            this->shortestPathDistance = distance;
        }
        int getLabel() {
            return label;
        }
        int getPathDistance() {
            return shortestPathDistance;
        }
        std::vector<Edge> getAdjacentEdges() {
            return adjacentEdges;
        }
        void printEdges() {
            for (auto edge : adjacentEdges) {
                std::cout << edge.to << " " << edge.weight << "\n";
            }
        }
};

class Graph {
    private:
        std::vector<Vertex> vertices;

    public:
        Graph() { }

        void addVertex(Vertex v) {
            vertices.push_back(v);
        }

        void dijkstraShortestPath(int start) {
            int currentVertex = start-1;
            // add all nodes to the unvisited set
            // all nodes that are not the start vertex are set to infinity
            // set start node to 0
            std::unordered_map<int, bool> unvisited;
            for (auto& v : vertices) {
                if (v.getLabel() == start) {
                    v.setPathDistance(0);
                } else {
                    v.setPathDistance(INT_MAX);
                }
                unvisited[v.getLabel()] = true;
            }

            bool done = false;
            while(vertices[currentVertex].getPathDistance() != INT_MAX && !done) {
                std::cout << "Current vertex: " << vertices[currentVertex].getLabel() << " with distance: " << vertices[currentVertex].getPathDistance() << "\n";
                // greedy shortest path criteria
                for (auto edge : vertices[currentVertex].getAdjacentEdges()) {
                    if(unvisited[edge.to]) {
                        if (vertices[currentVertex].getPathDistance() + edge.weight < vertices[edge.to-1].getPathDistance()) {
                            vertices[edge.to-1].setPathDistance(vertices[currentVertex].getPathDistance() + edge.weight);
                        }
                    }
                }
                // mark current vertex as visited
                unvisited[vertices[currentVertex].getLabel()] = false;

                done = true;
                // find the next unvisited vertex with the smallest path distance
                for (auto v : vertices) {
                    if (unvisited[v.getLabel()]) {
                        if (v.getPathDistance() < vertices[currentVertex].getPathDistance() || unvisited[vertices[currentVertex].getLabel()] == false) {
                            currentVertex = v.getLabel()-1;
                            done = false;
                        }
                    }
                }
            }
        }

        void print() {
            for (auto v : vertices) {
                std::cout << "Vertex " << v.getLabel() << ": " << v.getPathDistance() << std::endl;
            }
        }

        void printAssignmentAnswer() {
            std::vector<int> wanted = {7, 37, 59, 82, 99, 115, 133, 165, 188, 197};
            for (auto w : wanted) {
                std::cout << vertices[w-1].getPathDistance() << ",";
            }
        }
};

int main() {
    std::cout << "Enter File Name: ";
    std::string fileName;
    std::cin >> fileName;

    std::ifstream myfile (fileName);

    std::string line;

    Graph g;
    while (std::getline(myfile, line)) {
        // get vertex number
        Vertex v;
        std::string delimiter = "\t";
        std::string vertex = line.substr(0, line.find(delimiter));
        line.erase(0, line.find(delimiter) + delimiter.length());
        v.setLabel(std::stoi(vertex));

        // get all edges for this vertex
        while (!line.empty()) {
            std::string neighbor = line.substr(0, line.find(delimiter));
            line.erase(0, line.find(delimiter) + delimiter.length());
            std::string vertex2 = neighbor.substr(0, neighbor.find(','));
            neighbor.erase(0, neighbor.find(',') + 1);
            std::string weight = neighbor.substr(0, neighbor.find(','));
            v.addEdge(std::stoi(vertex2), std::stoi(weight));
        }

        // add vertex to graph
        g.addVertex(v);
    }
    std::cout << "Starting Shortest Path Algorithm" << "\n";
    g.dijkstraShortestPath(1);
    //g.print();
    g.printAssignmentAnswer();
}