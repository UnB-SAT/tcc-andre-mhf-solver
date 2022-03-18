#include "allMaxIndepSets.hpp"
#include "utils.hpp"
#include <iostream>

AllMaxIndependentSetsSolver::AllMaxIndependentSetsSolver(int numVariables, int numClausules, vector<set<int>> cnfGraph){
    this->numClausules = numClausules;
    this->numVariables = numVariables;
    this->cnfGraph = cnfGraph;
}

vector<set<int>> AllMaxIndependentSetsSolver::gerateAllMaxIndependentSets(){
    vector<set<int>> maxIdenpendentSets;
    set<int> tmp_next;
    // TODO maybe start with special vertex
    for(int i = 0; i < numClausules; i++){
        tmp_next.insert(i);
    }
    queue.insert(tmp_next);
    while(!queue.empty()){
        auto tmp_s = *queue.begin();
        if (*tmp_s.begin() > numClausules) {
            maxIdenpendentSets.push_back(tmp_s);
        }
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
    }
    return maxIdenpendentSets;
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