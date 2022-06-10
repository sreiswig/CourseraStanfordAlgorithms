#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>

struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<double> elapsed;
    
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        auto hrs = std::chrono::duration_cast<std::chrono::hours>(elapsed);
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(elapsed - hrs);
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed - hrs - mins);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed - hrs - mins - secs);
        std::cout << "Time elapsed: " << hrs.count() << ":" << mins.count() << ":" << secs.count() << ":" << ms.count() << " s\n";
    }
};

struct Clause {
    bool active = true;
    bool satisfied;
    std::vector<int> literals;
    Clause(std::vector<int> literals) : literals(literals) {}
};

class SATFormula {
    private:
        std::vector<Clause> clauses;
        std::unordered_map<int, bool> assignment;
        void checkIfSatisfied() {
            for (auto& clause : clauses) {
                clause.satisfied = false;
                for (auto literal : clause.literals) {
                    if (assignment[abs(literal)] == (literal < 0)) {
                        clause.satisfied = true;
                        break;
                    }
                }
            }
        }
    public:
        bool isSatisfied() {
            for (auto clause : clauses) {
                if (!clause.satisfied) {
                    return false;
                }
            }
            return true;
        }
        void setAssignment(std::unordered_map<int, bool> assignment) {
            this->assignment = assignment;
        }
        void flipAssignmentValue(int literal) {
            assignment[abs(literal)] = !assignment[abs(literal)];
            checkIfSatisfied();
        }
        void generateAllTrueAssignment() {
            auto gen = std::uniform_int_distribution<>(0,1);
            for (auto clause : clauses) {
                for (auto literal : clause.literals) {
                    assignment[abs(literal)] = true;
                }
            }
        }
        std::vector<Clause> getUnsatisfiedClauses() {
            std::vector<Clause> unsatisfiedClauses;
            for (auto clause : clauses) {
                if (!clause.satisfied) {
                    unsatisfiedClauses.push_back(clause);
                }
            }
            return unsatisfiedClauses;
        }
        void printAssignment() {
            for (auto a : assignment) {
                std::cout << a.first << ": " << a.second << "\n";
            }
        }
        SATFormula(std::vector<Clause> clauses) : clauses(clauses) {}
};

// split utility function
std::vector<std::string> split(std::string str, char delimeter) {
    std::string token;
    std::vector<std::string> result;
    while (str.find(delimeter) != std::string::npos) {
        token = str.substr(0, str.find(delimeter));
        str.erase(0, str.find(delimeter) + 1);
        if (token != "") {
            result.push_back(token);
        }
    }
    result.push_back(str);
    return result;
};

bool randKSAT(SATFormula phi, int num_steps) {
    Timer timer;
    std::default_random_engine generator;
    // initialize assignment a with a'
    phi.generateAllTrueAssignment();

    for (int i = 0; i < num_steps; i++) {
        if (!phi.isSatisfied()) {
            // choose an arbitrary clause C in phi that is not satisfied by a
            auto unsatisfiedClauses = phi.getUnsatisfiedClauses();
            std::uniform_int_distribution<int> distribution(0, unsatisfiedClauses.size() - 1);
            int randomIndex = distribution(generator);
            // choose a literal in C uniformly at random and change its assignment
            std::uniform_int_distribution<int> literalDistribution(0, unsatisfiedClauses[randomIndex].literals.size() - 1);
            int randomLiteralIndex = literalDistribution(generator);
            phi.flipAssignmentValue(unsatisfiedClauses[randomIndex].literals[randomLiteralIndex]);
            std::cout << "Flipped Literal: " << unsatisfiedClauses[randomIndex].literals[randomLiteralIndex] << "\n";
        } else {
            break;
        }
    }

    return phi.isSatisfied();
}

std::vector<Clause> getClausesFromFile(std::string filename) {
    std::vector<Clause> clauses;
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::vector<std::string> literals = split(line, ' ');
        std::vector<int> literals_int;
        for (int i = 0; i < literals.size(); i++) {
            literals_int.push_back(std::stoi(literals[i]));
        }
        Clause clause(literals_int);
        clauses.push_back(clause);
    }
    return clauses;
};

std::vector<Clause> reduceClauses(std::vector<Clause> clauses) {
    bool canReduce = true;
    int numLoops = 0;
    Timer timer;
    while (canReduce) {
        Timer loopTimer;
        std::unordered_map<int, std::pair<int, int>> literalOccurances;
        // according to so-far valid clauses, find positive and negative occurences of each literal
        for (auto clause : clauses) {
            if (clause.active) {
                for (auto literal : clause.literals) {
                    if (literal > 0) {
                        literalOccurances[literal].first++;
                    } else {
                        literalOccurances[-literal].second++;
                    }
                }
            }
        }
        
        canReduce = false;
        for (auto& clause : clauses) {
            if (clause.active) {
                for (auto& literal : clause.literals) {
                    if (literal > 0) {
                        if (literalOccurances[literal].first == 0 || literalOccurances[literal].second == 0) {
                            clause.active = false;
                            canReduce = true;
                        }
                    } else {
                        if (literalOccurances[-literal].first == 0 || literalOccurances[-literal].second == 0) {
                            clause.active = false;
                            canReduce = true;
                        }
                    }
                }
            }
        }

        std::vector<Clause> activeClauses;
        for (auto clause : clauses) {
            if (clause.active) {
                activeClauses.push_back(clause);
            }
        }

        numLoops++;
        std::cout << "Number of reduction loops: " << numLoops << "\n";
        std::cout << "Number of active clauses: " << activeClauses.size() << "\n";
        clauses = activeClauses;
    }
    return clauses;
}

int main() {
    std::cout << "Test Solution : y / n : ";
    std::string input;
    std::cin >> input;

    if (input == "y") {
        std::cout << "Enter file name: ";
        std::string fileName;
        std::cin >> fileName;
        std::ifstream file(fileName);
        std::string line;
        int numClauses;
        std::getline(file, line);
        numClauses = std::stoi(line);
        file.close();
        std::vector<Clause> clauses = getClausesFromFile(fileName);
        std::cout << "Number of clauses: " << numClauses << "\n";
        clauses = reduceClauses(clauses);
        std::cout << "Clauses reduced to : " << clauses.size() << "\n";
        numClauses = clauses.size();
        SATFormula phi(clauses);

        std::cout << "Save reduced clauses to file? y / n : ";
        std::cin >> input;
        if (input == "y") {
            std::cout << "Enter file name: ";
            std::string fileName;
            std::cin >> fileName;
            std::ofstream file(fileName);
            file << numClauses << "\n";
            for (auto clause : clauses) {
                if (clause.active) {
                    for (int i = 0; i < clause.literals.size(); i++) {
                        if (i == clause.literals.size() - 1) {
                            file << clause.literals[i];
                        } else {
                            file << clause.literals[i] << " ";
                        }
                    }
                    file << "\n";
                }
            }
            file.close();
        }
        std::cout << randKSAT(phi, 100) << "\n";
    } else {
        std::string fileHeader = "2sat";
        std::vector<bool> results;
        for (int i = 1; i < 7; i++) {
            std::string fileName = fileHeader + std::to_string(i) + "_reduced.txt";
            std::ifstream file(fileName);
            std::string line;
            int numClauses;
            std::getline(file, line);
            numClauses = std::stoi(line);
            file.close();
            std::vector<Clause> clauses = getClausesFromFile(fileName);
            SATFormula phi(clauses);
            results.push_back(randKSAT(phi, 10000));
        }
        for (auto result : results) {
            std::cout << std::boolalpha << result << " ";
        }
    }
}