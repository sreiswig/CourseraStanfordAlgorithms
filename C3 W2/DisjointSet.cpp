#include "DisjointSet.h"

// Make a new set with n elements
DisjointSet::DisjointSet(int n) {
    rank = new int[n];
    parent = new int[n];
    this->n = n;
    MakeSet();
}

// Initialize completely disjoint set with n elements
void DisjointSet::MakeSet() {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

// Find using simple recursion
int DisjointSet::Find(int x) {
    if (parent[x] != x) {
        parent[x] = Find(parent[x]);
    }
    return parent[x];
}

// Union by rank
void DisjointSet::Union(int x, int y) {
    int x_parent = Find(x);
    int y_parent = Find(y);

    if (x_parent == y_parent) {
        return;
    }

    if (rank[x_parent] > rank[y_parent]) {
        parent[y_parent] = x_parent;
    } else if (rank[x_parent] < rank[y_parent]) {
        parent[x_parent] = y_parent;
    } else {
        parent[y_parent] = x_parent;
        rank[x_parent]++;
    }
}
