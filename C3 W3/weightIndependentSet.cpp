#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

class WeightIndependentSet {
    private:
        std::vector<long long> weights;
        std::vector<long long> memoizationTable;

        long long computeMaximalWeightIndependentSet() {
            memoizationTable[0] = 0;
            memoizationTable[1] = weights[0];
            for (int i = 2; i < weights.size()+1; i++) {
                memoizationTable[i] = std::max(
                    memoizationTable[i - 1], 
                    memoizationTable[i - 2] + weights[i-1]
                    );
            }
            return memoizationTable.back();
        }

    public:
        WeightIndependentSet(std::vector<long long> weights) {
            this->weights = weights;
            memoizationTable = std::vector<long long>(weights.size()+1, 0);
            std::cout << "Max Value: " << computeMaximalWeightIndependentSet() << "\n";
        }

        void printMemoizationTable() {
            for (auto i : memoizationTable) {
                std::cout << i << " ";
            }
            std::cout << "\n";
        }

        void printWeights() {
            for (auto i : weights) {
                std::cout << i << " ";
            }
            std::cout << "\n";
        }

        std::vector<long long> getMIS() {
            std::vector<long long> mis;
            long long i = memoizationTable.size()-1;
            while ( i >= 1) {
                if (memoizationTable[i-1] >= memoizationTable[i-2] + weights[i-1]) {
                    i--;
                } else {
                    mis.push_back(i);
                    i -= 2;
                }
            }
            return mis;
        }

        std::string getAssignmentAnswer(std::vector<long long> interestedIndicies, std::vector<long long> mis) {
            std::string result = "";
            for (auto i : interestedIndicies) {
                if (std::find(mis.begin(), mis.end(), i) != mis.end()) {
                    result += "1";
                } else {
                    result += "0";
                }
            }
            return result;
        }
};

int main() {
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);
    int num_vertices = std::stoi(line);

    std::vector<long long> weights;
    int label = 1;
    while(std::getline(file, line)) {
        weights.push_back(std::stoll(line));
        label++;
    }

    WeightIndependentSet wi(weights);
    std::cout << wi.getAssignmentAnswer({1,2,3,4,17,117,517,997}, wi.getMIS()) << std::endl;
}