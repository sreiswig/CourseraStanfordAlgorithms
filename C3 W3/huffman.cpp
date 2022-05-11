#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <vector>

struct Vertex {
    long long label;
    Vertex *parent;
    Vertex *left_child;
    Vertex *right_child;
};

struct CmpVertexPtrs {
    bool operator()(const Vertex *lhs, const Vertex *rhs) const {
        return lhs->label > rhs->label;
    }
};

class HuffmanTree {
    private:
        Vertex *root;
        std::vector<Vertex*> vertices;

        void printSubTree(Vertex *v) {
            std::cout << v->label << " ";
            if(v->left_child != NULL) {
                printSubTree(v->left_child);
            } 
            if (v->right_child != NULL) {
                printSubTree(v->right_child);
            }
        }

        int getMinPath(Vertex *v) {
            if (v->left_child == NULL && v->right_child == NULL) {
                return 0;
            } else if (v->left_child == NULL) {
                return getMinPath(v->right_child) + 1;
            } else if (v->right_child == NULL) {
                return getMinPath(v->left_child) + 1;
            } else {
                return std::min(getMinPath(v->left_child), getMinPath(v->right_child)) + 1;
            }
        }

        int getMaxPath(Vertex *v) {
            if (v->left_child == NULL && v->right_child == NULL) {
                return 0;
            } else if (v->left_child == NULL) {
                return getMaxPath(v->right_child) + 1;
            } else if (v->right_child == NULL) {
                return getMaxPath(v->left_child) + 1;
            } else {
                return std::max(getMaxPath(v->left_child), getMaxPath(v->right_child)) + 1;
            }
        }

        void printCode(Vertex *v, std::string code) {
            if (v->left_child == NULL && v->right_child == NULL) {
                std::cout << v->label << " " << code << std::endl;
            } 
            if (v->left_child != NULL) {
                printCode(v->left_child, code + "0");
            }
            if (v->right_child != NULL) {
                printCode(v->right_child, code + "1");
            }
        }

    public:
        void setVertices(std::vector<Vertex*> vertices) {
            this->vertices = vertices;
        }
        void createTree() {
            std::priority_queue<Vertex*, std::vector<Vertex*>, CmpVertexPtrs> pq;
            for (auto v : vertices) {
                pq.push(v);
            }
            while (pq.size() > 1) {
                Vertex *v1 = pq.top();
                pq.pop();
                Vertex *v2 = pq.top();
                pq.pop();
                Vertex *v3 = new Vertex;
                v3->label = v1->label + v2->label;
                v3->parent = NULL;
                v3->left_child = v1;
                v3->right_child = v2;
                pq.push(v3);
            }
            root = pq.top();
        }
        void printMaxMinPath() {
            std::cout << "Max path: " << getMaxPath(root) << "\n";
            std::cout << "Min path: " << getMinPath(root) << "\n";
        }
        void printTree() {
            printSubTree(root);
            std::cout << "\n";
        }
        void printCode() {
            printCode(root, "");
            std::cout << "\n";
        }
        void printQueue() {
            std::priority_queue<Vertex*, std::vector<Vertex*>, std::greater<Vertex*>> pq;
            for (auto v : vertices) {
                pq.push(v);
            }
            while (!pq.empty()) {
                Vertex *v = pq.top();
                pq.pop();
                std::cout << v->label << " ";
            }
        }
};

int main() {
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;

    std::ifstream file(filename);
    std::string line;

    int num_symbols = 0;
    std::getline(file, line);
    num_symbols = std::stoi(line);

    HuffmanTree ht;
    std::vector<Vertex*> vertices;
    long long total = 0;
    while(std::getline(file, line)) {
        long long cost = std::stoll(line);
        Vertex *v = new Vertex;
        v->label = cost;
        v->parent = NULL;
        v->left_child = NULL;
        v->right_child = NULL;
        vertices.push_back(v);
        total += cost;
    }
    std::cout << "Total: " << total << "\n";
    ht.setVertices(vertices);
    ht.createTree();
    ht.printTree();
    ht.printCode();
    ht.printMaxMinPath();
    ht.printQueue();
}