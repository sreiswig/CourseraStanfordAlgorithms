#include "DisjointSet.cpp"
#include <iostream>

int main() {
    DisjointSet ds(5);

    for (int i = 0; i < 5; i++) {
        std::cout << "Expected: " << i << " Actual: " << ds.Find(i) << "\n";
    }

    ds.Union(0, 1);
    ds.Union(1, 2);
    std::cout << "Expected: 0 Actual: " << ds.Find(0) << "\n";
    std::cout << "Expected: 0 Actual: " << ds.Find(1) << "\n";
}