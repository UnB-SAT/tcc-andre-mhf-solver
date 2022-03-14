#ifndef _HORNSOLVER_HPP
#define _HORNSOLVER_HPP

#include <set>
#include <vector>
#include <map>

using namespace std;

class HornSolver{
public:
    HornSolver();

    bool solveHornSat(int numVariables, vector<set<int>> clausules, set<int> initialValues);
    void printAnswer();
private:
    map<int, bool> values;
    int numVariables;

    vector<set<int>> clausules;

    int setValue(int value);
};

#endif