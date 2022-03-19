#ifndef _ALL_MAX_INDEP_SETS_H
#define _ALL_MAX_INDEP_SETS_H

#include <vector>
#include <set>

#include "parser.hpp"

using namespace std;

class AllMaxIndependentSetsSolver{
public:
    AllMaxIndependentSetsSolver(int numVariables, int numClausules, vector<set<int>> cnfGraph);
    
    set<int> gerateAllMaxIndependentSets();
private:
    int numVariables;
    int numClausules;
    vector<set<int>> cnfGraph;
    set<set<int>> queue;

    bool isAdjacent(int a, int b);
    bool isMaximal(set<int> potentialSet);
};

#endif