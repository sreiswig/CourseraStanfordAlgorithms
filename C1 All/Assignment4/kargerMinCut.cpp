#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>

std::default_random_engine gen;

class Vertex {
    private:
        int label;
        std::vector<int> edges;

    public:
        int GetLabel() {return label;}

        int GetNumberOfEdges() {
            return edges.size();
        }

        int GetEdge(int index) {
            return edges.at(index);
        }

        int GetNumberOfEdgesToVertex(int vertex) {
            int numEdges = 0;
            for (auto edge : edges) {
                if(edge == vertex) {
                    numEdges++;
                }
            }
            return numEdges;
        }

        std::vector<int> GetEdges() {
            return edges;
        }
        
        void AddEdge(int edge) {
            edges.push_back(edge);
        }

        void AddEdges(std::vector<int> edgesToConcat) {
            edges.insert(edges.end(), edgesToConcat.begin(), edgesToConcat.end());
        }

        void RemoveEdgesToVertex(int vertex) {
            auto found = std::find(edges.begin(), edges.end(), vertex);
            while (found != edges.end()) {
                edges.erase(found);
                found = std::find(edges.begin(), edges.end(), vertex);
            }
        }

        void RemoveEdgeAtIndex(int index) {
            edges.erase(edges.begin() + index);
        }

        Vertex(std::vector<int> e, int l) {
            edges = e;
            label = l;
        }

        Vertex(int label) {
            this->label  = label;
        }

        void PrintEdges() {
            std::cout << "edges of vertex " << label << "\n"; 
            for (auto edge : edges) {
                std::cout << edge << " ";
            }
            std::cout << "\n";
        }

        void RemoveSelfLoops() {
            auto found = std::find(edges.begin(), edges.end(), label);
            while (found != edges.end()) {
                edges.erase(found);
                found = std::find(edges.begin(), edges.end(), label);
            }
        }

        int RemoveEdgeAtRandom() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, edges.size() - 1);
            int randomEdge = distrib(gen);
            int edgeValue = edges.at(randomEdge);
            RemoveEdgeAtIndex(randomEdge);
            return edgeValue;
        }

        friend bool operator== (const Vertex &n1, const Vertex &n2);
};

bool operator== (const Vertex &n1, const Vertex &n2) {
    return n1.label == n2.label;
};

class Graph {
    private:
        std::vector<Vertex> vertices;

    public:
        Graph(std::vector<Vertex> n) {
            vertices = n;
        }

        int KargerMinCut() {
            std::vector<Vertex> kargerVertices = vertices; // copy the vertices so that we don't lose them during min cut
            while (kargerVertices.size() > 2) {
                // select a vertex at random
                std::uniform_int_distribution<> distrib(0, kargerVertices.size() - 1);
                int randomVertex = distrib(gen);

                // get vertex and select a random edge, and remove it.
                int otherVertexValue = kargerVertices.at(randomVertex).RemoveEdgeAtRandom();

                // add all edges from vertex
                auto otherVertex = std::find(kargerVertices.begin(), kargerVertices.end(), Vertex(otherVertexValue));
                auto otherEdges = otherVertex->GetEdges();
                kargerVertices.at(randomVertex).AddEdges(otherEdges);
                for (auto edge : otherEdges) {
                    auto addVertex = std::find(kargerVertices.begin(), kargerVertices.end(), Vertex(edge));
                    addVertex->AddEdge(kargerVertices.at(randomVertex).GetLabel());
                }

                // remove otherVertex from all other vertices
                for (Vertex& vertex : kargerVertices) {
                    vertex.RemoveEdgesToVertex(otherVertexValue);
                }

                // remove self loops
                kargerVertices.at(randomVertex).RemoveSelfLoops();

                // erase vetex from vertices
                kargerVertices.erase(otherVertex);
            }

            return kargerVertices.at(0).GetEdges().size();
        }

        void Print() {
            for (auto vertex : vertices) {
                vertex.PrintEdges();
            }
        }
};

int main() {
    // load values into vector int
    std::string fileName = "kargerMinCut.txt";
    std::string line;
    std::ifstream myfile ("kargerMinCut.txt");
    std::vector<int> nums;
    std::vector<Vertex> vertices;

    int expectedAnswer;
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            std::vector<int> nodeAsIntVector;
            
            std::istringstream iss(line);
            std::string token;
            
            if(fileName == "testCase1.txt") {
                while (std::getline(iss, token, ' ')) {
                    if(token == "expected") {
                        std::getline(iss, token, ' ');
                        expectedAnswer = stoi(token);
                    } else {
                        nodeAsIntVector.push_back(stoi(token));
                    }
                }
            } else {
                while (std::getline(iss, token, '\t')) {
                    nodeAsIntVector.push_back(stoi(token));
                }
            }

            if (nodeAsIntVector.size() > 0) {
                std::vector<int> edges;
                for (auto it = nodeAsIntVector.begin() + 1; it != nodeAsIntVector.end(); it++) {
                    edges.push_back(*it);
                }
                Vertex currentVertex = Vertex(edges, nodeAsIntVector.at(0));
                vertices.push_back(currentVertex);
            }
        }
        myfile.close(); 
    }
    
    Graph G = Graph(vertices);
    int minCut = INT32_MAX;
    for (int i = 0; i < 100; i++) {
        int cut = G.KargerMinCut();
        std::cout << "Current Cut: " << cut << "\n";
        if(cut < minCut) {
            minCut = cut;
        }
    }
    std::cout << "Min Cut " << minCut << "\n";

    return 0;
}