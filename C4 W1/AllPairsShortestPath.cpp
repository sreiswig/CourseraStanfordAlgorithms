#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
        std::vector<Edge> edges;
    public:
        Graph(int numVertices, int numEdges, std::vector<Edge> edges) {
            this->numVertices = numVertices;
            this->numEdges = numEdges;
            this->edges = edges;            
        }

        void djikstraShortestPath() {

        }

        void bellmanFord(int start) {

        }

        void johnsonsAlgorithm() {
            int extraVertex = numVertices + 1;
            std::vector<Edge> extraEdges;
            for (int i = 1; i <= numVertices; i++) {
                Edge e = {extraVertex, i, 0};
                extraEdges.push_back(e);
            }
        }

        long long minShortestPath() {

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

    return 0;
}