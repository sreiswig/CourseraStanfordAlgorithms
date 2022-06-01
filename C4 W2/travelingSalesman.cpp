#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

// cartesian point
struct point {
    double x, y;
    point(double x, double y) : x(x), y(y) {}
};

// euclidian distance between points
double EuclideanDistance(point a, point b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// split utility function
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

double travelingSalesman(std::vector<std::vector<double>>& distances, int mask, int source, int visited) {
    if (visited == mask) {
        return distances[source][0];
    }
    double result = INT32_MAX;

    // iterate over all unvisited nodes
    for (int i = 0; i < distances.size(); i++) {
        if (((1 << i) & mask) == 0) {
            result = std::min(result, distances[source][i] + travelingSalesman(distances, mask | (1 << i), i, visited));
        }
    }
    return result;
}

std::vector<std::vector<double>> getDistancesFromFile(std::string filename) {
    std::ifstream file(filename);
    std::string line;

    std::getline(file, line);
    char delimeter = ' ';

    // get number of cities
    int numCities = std::stoi(line);

    // get cities as points
    std::vector<point> cities;
    while (std::getline(file, line)) {
        double x, y;
        std::vector<std::string> line_split = split(line, delimeter);
        x = std::stod(line_split[0]);
        y = std::stod(line_split[1]);
        cities.push_back(point(x, y));
    }

    // precomputed distances lookup table
    std::vector<std::vector<double>> distances(numCities, std::vector<double>(numCities));
    for (int i = 0; i<numCities; i++) {
        for (int j = 0; j<numCities; j++) {
            distances[i][j] = EuclideanDistance(cities[i], cities[j]);
        }
    }
    return distances;
}

int main() {
    std::cout << "Split y / n: ";
    std::string answer;
    std::cin >> answer;
    if (answer == "y") {
        std::cout << "Enter left split file: ";
        std::string leftFile;
        std::cin >> leftFile;

        std::cout << "Enter right split file: ";
        std::string rightFile;
        std::cin >> rightFile;

        auto leftDistances = getDistancesFromFile(leftFile);
        auto rightDistances = getDistancesFromFile(rightFile);

        int leftNumCities = leftDistances.size();
        int rightNumCities = rightDistances.size();

        int leftMask = (1 << leftNumCities) - 1;
        int rightMask = (1 << rightNumCities) - 1;
        
        double leftResult = travelingSalesman(leftDistances, 1, 0, leftMask);
        double rightResult = travelingSalesman(rightDistances, 1, 0, rightMask);
        std::cout << "Left: " << leftResult << std::endl;
        std::cout << "Right: " << rightResult << std::endl;
        std::cout << "Total: " << std::floor(leftResult + rightResult - (2*leftDistances[11][12])) << std::endl;
    } else {
        // read file
        std::cout << "Enter filename: ";
        std::string filename;
        std::cin >> filename;

        auto distances = getDistancesFromFile(filename);
        int numCities = distances.size();

        for (int i = 0; i < numCities; i++) {
            for (int j = 0; j < numCities; j++) {
                std::cout << distances[i][j] << " ";
            }
            std::cout << std::endl;
        }

        // The discussion forums say to do bitmasking
        int visited_mask = (1 << numCities) - 1;
        std::cout << "shortest path: " << travelingSalesman(distances, 1, 0, visited_mask) << "\n";
    }
}