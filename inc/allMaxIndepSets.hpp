#ifndef _ALL_MAX_INDEP_SETS_H
#define _ALL_MAX_INDEP_SETS_H

#include <vector>
#include <set>

#include "parser.hpp"

using namespace std;

class AllMaxIndependentSetsSolver{
public:
    AllMaxIndependentSetsSolver(Parser* parser);
    
    vector<set<int>> gerateAllMaxIndependentSets();
private:
    Parser* parser;
    set<set<int>> queue;

    bool isAdjacent(int a, int b);
    bool isMaximal(set<int> potentialSet);
};

#endif