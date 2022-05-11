class DisjointSet {
    private:
        int *rank, *parent, n;
    public:
        DisjointSet(int n);
        void MakeSet();
        int Find(int x);
        void Union(int x, int y);
};