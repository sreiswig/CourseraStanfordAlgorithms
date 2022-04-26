#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

int main() {
    std::cout << "Enter File Name: ";
    std::string fileName;
    std::cin >> fileName;

    std::cout << "Enter lower bound: ";
    long long int lowerBound;
    std::cin >> lowerBound;

    std::cout << "Enter upper bound: ";
    long long int upperBound;
    std::cin >> upperBound;

    std::ifstream file(fileName);
    std::string line;

    std::unordered_set<long long int> numsAvailable;

    // read all into has table
    while (std::getline(file, line)) {
        long long int num = std::stoll(line);
        numsAvailable.insert(num);
    }

    int total = 0;
    for (long long int i = lowerBound; i < upperBound+1; i++) {
        for (auto it = numsAvailable.begin(); it != numsAvailable.end(); it++) {
            if (numsAvailable.find(i - *it) != numsAvailable.end()) {
                total++;
                break;
            }
        }
        std::cout << "i: " << i << " current total: " << total << "\n";
    }
    std::cout << "Total: " << total << "\n";
}