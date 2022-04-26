#include <iostream>
#include <fstream>
#include <string>
#include <queue>

int main() {
    std::cout << "Enter File Name: ";
    std::string fileName;
    std::cin >> fileName;

    std::ifstream file(fileName);
    std::string line;

    std::priority_queue<int, std::vector<int>, std::greater<int>> high;
    std::priority_queue<int> low;

    int medianSum = 0;

    while(std::getline(file, line)) {
        int num = std::stoi(line);

        if (high.size() == 0 && low.size() == 0) {
            medianSum += num;
            high.push(num);
        } else if (high.size() > low.size()) {
            if (num > high.top()) {
                low.push(high.top());
                high.pop();
                high.push(num);
                medianSum += low.top();
            } else {
                low.push(num);
                medianSum += low.top();
            }
        } else {
            if (num < low.top()) {
                high.push(low.top());
                low.pop();
                low.push(num);
                medianSum += high.top();
            } else {
                high.push(num);
                medianSum += high.top();
            }
        }
    }
    std::cout << medianSum % 10000 << "\n";
}