#include "allMaxIndepSets.hpp"
#include "utils.hpp"
#include <iostream>

AllMaxIndependentSetsSolver::AllMaxIndependentSetsSolver(int numVariables, int numClausules, vector<set<int>> cnfGraph){
    this->numClausules = numClausules;
    this->numVariables = numVariables;
    this->cnfGraph = cnfGraph;

    set<int> tmp_next;

    for(int i = 0; i < numClausules; i++){
        tmp_next.insert(i);
    }
    queue.insert(tmp_next);
}

set<int> AllMaxIndependentSetsSolver::gerateAllMaxIndependentSets(){
    while(!queue.empty()){
        auto tmp_s = *queue.begin();
        for(size_t j = 0; j < cnfGraph.size(); j++){
            for(auto i : tmp_s){
                if(isAdjacent(i,j) && (size_t)i < j){
                    
                    set<int> sj; 
                    for(auto x : tmp_s){
                        if((size_t)x <= j){
                            sj.insert(x);
                        }
                    }

                    auto tj = cnfGraph[j];

                    for(auto x : tj){
                        auto xPos = sj.find(x);
                        if(xPos != sj.end()){
                            sj.erase(xPos);
                        }
                    }

                    sj.insert(j);

                    if(isMaximal(sj)){
                        queue.insert(sj);
                        break;
                    }
                }
            }
        }
        queue.erase(queue.begin());
        if (*tmp_s.begin() > numClausules) {
            return tmp_s;
        }
    }
    return {};
}

bool AllMaxIndependentSetsSolver::isAdjacent(int a, int b){
    return cnfGraph[a].find(b) != cnfGraph[a].end();
}

bool AllMaxIndependentSetsSolver::isMaximal(set<int> potentialSet){
    set<int> visibleClausules;
    for(auto x : potentialSet){
        if (x < numClausules) {
            visibleClausules.insert(x);
        }
        else if (x > numClausules) {
            for(auto y : cnfGraph[x]){
                if(y < numClausules){
                    visibleClausules.insert(y);
                }
            }
        }
    }
    return visibleClausules.size() == (size_t)numClausules;
}