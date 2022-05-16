#include <iostream>
#include <fstream>
#include <string>
#include <vector>

long long knapsack(std::vector<long long> &weights, std::vector<long long> &values, long long capacity) {
    long long n = weights.size();
    std::vector<std::vector<long long>> dp(n + 1, std::vector<long long>(capacity + 1, 0));
    for (long long i = 1; i <= n; i++) {
        for (long long j = 1; j <= capacity; j++) {
            if (weights[i - 1] <= j) {
                dp[i][j] = std::max(dp[i - 1][j], dp[i - 1][j - weights[i - 1]] + values[i - 1]);
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    return dp[n][capacity];
}

int main() {
    std::cout << "Enter Filename: ";
    std::string filename;
    std::cin >> filename;

    std::ifstream file(filename);

    std::string line;
    std::string delimeter = " ";

    long long knapsack_capacity, number_of_items;
    std::getline(file, line);
    std::string token = line.substr(0, line.find(delimeter));
    line.erase(0, line.find(delimeter) + delimeter.length());
    knapsack_capacity = std::stoll(token);
    number_of_items = std::stoll(line);

    std::vector<long long> weights;
    std::vector<long long> values;
    while(std::getline(file, line)) {
        token = line.substr(0, line.find(delimeter));
        line.erase(0, line.find(delimeter) + delimeter.length());
        values.push_back(std::stoll(token));
        weights.push_back(std::stoll(line));
    }
    std::cout << knapsack(weights, values, knapsack_capacity) << std::endl;
}