#include <vector>
#include <iostream>

using namespace std;

class TwoCnfSolver{
    private:
        int n;
        vector<vector<int>> g, gt;
        vector<bool> used;
        vector<int> order, comp;
        vector<bool> assignment;

    public:
        TwoCnfSolver(int n, vector<vector<int>> g, vector<vector<int>> gt);
        bool solve_2SAT();
        vector<int> getAnswer();

    private:
        void dfs1(int v);
        void dfs2(int v, int cl);
};