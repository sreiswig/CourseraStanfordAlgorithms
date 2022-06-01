#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <set>

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

double travelingSalesmanNN(std::vector<std::vector<double>> distances) {
    std::set<int> unvisited;
    for (int i = 1; i < distances.size(); i++) {
        unvisited.insert(i);
    }
    int current = 0;
    double totalDistance = 0;
    while (unvisited.size() > 0) {
        int next = -1;
        double minDistance = -1;
        for (auto city : unvisited) {
            if (minDistance == -1 || distances[current][city] < minDistance) {
                minDistance = distances[current][city];
                next = city;
            }
        }
        unvisited.erase(next);
        totalDistance += minDistance;
        current = next;
    }
    return totalDistance + distances[current][0];
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
        x = std::stod(line_split[1]);
        y = std::stod(line_split[2]);
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
    // read file
    std::cout << "Enter filename: ";
    std::string filename;
    std::cin >> filename;

    auto distances = getDistancesFromFile(filename);
    int numCities = distances.size();
    
    std::cout << std::fixed << "shortest path: " << travelingSalesmanNN(distances) << "\n";
}