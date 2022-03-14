#ifndef _SOLVER_HPP
#define _SOLVER_HPP

#include <set>
#include <vector>
#include "parser.hpp"

using namespace std;

class Solver{
public:
    Solver(Parser* parser);

    void solveSolo();
    void solve2Cnf();
    void solveHorn();
    void solveMHF();
private:
    Parser* parser;

    int getLitPos(int lit);
    void insertIntoGraph(vector<set<int>>* graph, int a, int b);
};

#endif