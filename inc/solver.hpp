#ifndef _SOLVER_HPP
#define _SOLVER_HPP

#include <set>
#include <vector>

using namespace std;

class Solver{
public:
    Solver();

    void solveSolo(set<int> lits);
    void solve2Cnf(int numVariables, vector<set<int>> clausules, set<int> soloLiterals);
    void solveHorn(int numVariables, vector<set<int>> clausules, set<int> initialValues);
    void solveMHF();
};

#endif