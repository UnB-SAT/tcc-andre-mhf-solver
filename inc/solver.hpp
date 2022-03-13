#ifndef _SOLVER_HPP
#define _SOLVER_HPP

#include <set>

using namespace std;

class Solver{
public:
    Solver();

    void solveSolo(set<int> lits);
    void solve2Cnf(int n, vector<vector<int>> g, vector<vector<int>> gt);
    void solveHorn();
    void solveMHF();
};

#endif