#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <vector>
#include <algorithm>
#include "DisjointSet.cpp"

struct Edge {
    int v1;
    int v2;
    int cost;

    inline bool operator<(const Edge& rhs) const {
        return cost < rhs.cost;
    }
};

class Graph {
    private:
        int num_vertices;
        std::vector<Edge> edges;
    public:
        Graph(int num_vertices) {
            this->num_vertices = num_vertices;
        }

        void addEdge(int v1, int v2, int cost) {
            Edge e;
            e.v1 = v1;
            e.v2 = v2;
            e.cost = cost;
            edges.push_back(e);
        }
        std::vector<Edge> getEdges() {
            return edges;
        }

        std::vector<Edge> kruskal() {
            std::vector<Edge> mst;
            DisjointSet ds(num_vertices);
            std::sort(edges.begin(), edges.end());
            for (auto e : edges) {
                if (ds.Find(e.v1) != ds.Find(e.v2)) {
                    mst.push_back(e);
                    ds.Union(e.v1, e.v2);
                }
            }
            return mst;
        }

        long long k_clustering(int k) {
            int num_clusters = num_vertices;
            int max = 0;
            DisjointSet ds(num_vertices);
            std::sort(edges.begin(), edges.end());
            for (auto e : edges) {
                if (num_clusters >= k) {
                    if (ds.Find(e.v1-1) != ds.Find(e.v2-1)) {
                        ds.Union(e.v1-1, e.v2-1);
                        num_clusters--;
                        if (num_clusters < k) {
                            return e.cost;
                        }
                    }
                } 
            }
            return max;
        }

        long long big_clustering(int minDistance) {

        }

        void printEdges() {
            std::sort(edges.begin(), edges.end());
            for (auto e : edges) {
                std::cout <<"v1: " << e.v1 << " v2: " << e.v2 << " cost: " << e.cost << "\n";
            }
        }
};

int main() {
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);
    std::string delimeter = " ";
    std::string token = line.substr(0, line.find(delimeter));
    line.erase(0, line.find(delimeter) + delimeter.length());
    
    int num_vertices = std::stoi(token);
    Graph g(num_vertices);

    // Read from different types of files depending of the structure of the first line of the file
    if (line.empty()) {
        while (std::getline(file, line)) {
            int v1, v2, cost;
            token = line.substr(0, line.find(delimeter));
            line.erase(0, line.find(delimeter) + delimeter.length());
            v1 = std::stoi(token);
            token = line.substr(0, line.find(delimeter));
            line.erase(0, line.find(delimeter) + delimeter.length());
            v2 = std::stoi(token);
            token = line.substr(0, line.find(delimeter));
            line.erase(0, line.find(delimeter) + delimeter.length());
            cost = std::stoi(token);
            g.addEdge(v1, v2, cost);
        }
        std::cout << "K = 2: " << g.k_clustering(2) << "\n";
        std::cout << "K = 3: " << g.k_clustering(3) << "\n";
        std::cout << "K = 4: " << g.k_clustering(4) << "\n";
    } else {
        line.erase(0, line.find(delimeter) + delimeter.length());
        token = line.substr(0, line.find(delimeter));
        int num_bits_per_vertex = std::stoi(token);
        
        std::bitset<24> bits; // 24 bits are from the assignment description
        while (std::getline(file, line)) {
            for (int i = 0; i < num_bits_per_vertex; i++) {
                token = line.substr(0, line.find(delimeter));
                line.erase(0, line.find(delimeter) + delimeter.length());
                if (token == "1") {
                    bits.set(i);
                } else {
                    bits.reset(i);
                }
            }
            vertex_bits.push_back(bits);
        }
    }
}