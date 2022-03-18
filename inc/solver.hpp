#ifndef _SOLVER_HPP
#define _SOLVER_HPP

#include <set>
#include <vector>
#include "parser.hpp"

using namespace std;

class Solver{
public:
    Solver(Parser* parser);

    int returnValue;
    vector<int> answer;

    void solveSolo();
    void solve2Cnf();
    void solveHorn();
    void solveMHF();

    void generateNewFormula(clock_t startTime, clock_t endTime);
private:
    Parser* parser;

    int getLitPos(int lit);
    void insertIntoGraph(vector<set<int>>* graph, int a, int b);
};

#endif