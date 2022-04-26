#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <stack>
#include <string>

class Vertex {
    private:
        bool visited;
        bool reverseVisited;
        bool visitedAllChildren;
        bool forwardVisited;
        std::vector<int> edges;
        std::vector<int> reverseEdges;

    public:
        Vertex() {
            visited = false;
            reverseVisited = false;
            visitedAllChildren = false;
            forwardVisited = false;
        }

        void SetVisited(bool visited) {
            this->visited = visited;
        }

        void SetReverseVisited(bool reverseVisited) {
            this->reverseVisited = reverseVisited;
        }

        void SetVisitedAllChildren(bool visitedAllChildren) {
            this->visitedAllChildren = visitedAllChildren;
        }

        void SetForwardVisited(bool forwardVisited) {
            this->forwardVisited = forwardVisited;
        }

        void AddEdge(int edge) {
            edges.push_back(edge);
        }

        void AddReverseEdge(int edge) {
            reverseEdges.push_back(edge);
        }

        bool GetVisited() {
            return visited;
        }  

        bool GetReverseVisited() {
            return reverseVisited;
        }

        bool GetVisitedAllChildren() {
            return visitedAllChildren;
        }

        bool GetForwardVisited() {
            return forwardVisited;
        }

        std::vector<int> GetEdges() {
            return edges;
        }

        std::vector<int> GetReverseEdges() {
            return reverseEdges;
        }
};

class Graph {
    private:
        std::map<int, Vertex> vertices;
        std::vector<int> scc;
        std::vector<int> finishingOrder;
        int currentFinishingTime = 0;

    public:
        void SetVertex(int label, Vertex v) {
            vertices[label] = v;
        }

        void AddEdge(int head, int tail) {
            vertices[tail].AddEdge(head);
            vertices[head].AddReverseEdge(tail);
        }

        void kosarajuFirstPass(int vertex) {
            if (vertices[vertex].GetReverseVisited()) {
                return;
            }
            vertices[vertex].SetReverseVisited(true);
            for (auto edge : vertices[vertex].GetReverseEdges()) {
                if (!vertices[edge].GetReverseVisited()) {
                    kosarajuFirstPass(edge);
                }
            }
            finishingOrder.push_back(vertex);
        }

        void kosarajuSecondPass(int vertex, int root) {
            if (vertices[vertex].GetVisited()) {
                return;
            }
            vertices[vertex].SetVisited(true);
            for (auto edge : vertices[vertex].GetEdges()) {
                if (!vertices[edge].GetVisited()) {
                    kosarajuSecondPass(edge, root);
                }
            }
            scc[root]++;
        }

        void kosarajusAlgorithm() {
            for (auto vertex : vertices) {
                if (!vertex.second.GetReverseVisited()) {
                    kosarajuFirstPass(vertex.first);
                }
            }
            for (int i = finishingOrder.size() - 1; i >= 0; --i) {
                if (!vertices[i].GetVisited()) {
                    kosarajuSecondPass(i, i);
                }
            }
        }

        void stackKosarajusAlgorithm() {
            std::stack<int> stack;
            std::vector<int> finishingVector;

            // for every vertex in the graph including disconnected components
            for (auto vertex : vertices) {
                // if the vertex has not been visited
                if (!vertex.second.GetReverseVisited()) {
                    // push the vertex onto the stack, this becomes the first vertex in the dfs
                    stack.push(vertex.first);
                    int index = 0;
                    // while the stack is not empty
                    while (!stack.empty()) {
                        // if the vertex has not been visited
                        if (!vertices[stack.top()].GetVisitedAllChildren()) {
                            // set the vertex to visited
                            vertices[stack.top()].SetReverseVisited(true);

                            bool visitedAllChildren = true;
                            // for every outgoing edge of the vertex
                            for (int i = index; i < vertices[stack.top()].GetReverseEdges().size(); i++) {
                                if (!vertices[vertices[stack.top()].GetReverseEdges()[i]].GetReverseVisited()) {
                                    // push the vertex onto the stack
                                    stack.push(vertices[stack.top()].GetReverseEdges()[i]);
                                    visitedAllChildren = false;
                                    break;
                                }
                            }
                            vertices[stack.top()].SetVisitedAllChildren(visitedAllChildren);
                        } else {
                            // the vertex has been visited before so we can pop it off the stack
                            finishingVector.push_back(stack.top());
                            stack.pop();
                        }
                    }
                }
            }

            // while the finishing stack is not empty
            for (int i = finishingVector.size() - 1; i >= 0; i--) {
                int root = finishingVector[i];
                scc.push_back(0);
                if (!vertices[root].GetVisited()) {
                    stack.push(root);
                }
                while (!stack.empty()) {
                    if (!vertices[stack.top()].GetForwardVisited()) {
                        vertices[stack.top()].SetVisited(true);

                        bool visitedAllChildren = true;
                        for (auto edge : vertices[stack.top()].GetEdges()) {
                            if (!vertices[edge].GetVisited()) {
                                stack.push(edge);
                                visitedAllChildren = false;
                                break;
                            }
                        }
                        vertices[stack.top()].SetForwardVisited(visitedAllChildren);
                    } else {
                        stack.pop();
                        scc[scc.size() - 1]++;
                    }
                }
            }
        }

        void print() {
            for (auto vertex : vertices) {
                std::cout << vertex.first << ": ";
                for (auto edge : vertex.second.GetEdges()) {
                    std::cout << edge << " ";
                }
                std::cout << "\n";
            }
        }

        void printReverse() {
            for (auto vertex : vertices) {
                std::cout << vertex.first << ": ";
                for (auto edge : vertex.second.GetReverseEdges()) {
                    std::cout << edge << " ";
                }
                std::cout << "\n";
            }
        }

        void printFinishingTimes() {
            for (auto vertex : finishingOrder) {
                std::cout << vertex << " ";
            }
            std::cout << "\n";
        }

        void printSCC() {
            sort(scc.rbegin(), scc.rend());
            for (int i = 0; i < 5; i++) {
                std::cout << scc[i] << ",";
            }
            std::cout << "\n";
        }
};

int main() {
    std::cout << "Enter File Name: ";
    std::string fileName;
    std::cin >> fileName;

    if (fileName == "test") {
        for ( int i = 1; i < 8; i++) {
            fileName = "test_input" + std::to_string(i) + ".txt";
            std::ifstream myfile (fileName);

            Graph G = Graph();

            if (myfile.is_open()) {
                int tail;
                int head;
                while (myfile >> tail >> head) {
                    G.AddEdge(head, tail);
                }
            }

            myfile.close();
            G.stackKosarajusAlgorithm();
            G.printSCC();

            fileName = "test_answer" + std::to_string(i) + ".txt";
            std::ifstream myfile2 (fileName);
            if (myfile2.is_open()) {
                std::string answer;
                while (myfile2 >> answer) {
                    std::cout << answer << " ";
                }
                std::cout << "\n";
            }
            myfile2.close();
        }
    } else {
        std::ifstream myfile (fileName);

        Graph G = Graph();

        if (myfile.is_open()) {
            int tail;
            int head;
            while (myfile >> tail >> head) {
                G.AddEdge(head, tail);
            }
        }

        myfile.close();
        G.stackKosarajusAlgorithm();
        //G.kosarajusAlgorithm();
        G.printSCC();
    }
}