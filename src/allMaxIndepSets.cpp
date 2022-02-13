#include "allMaxIndepSets.hpp"
#include <iostream>

using namespace std;

AllMaxIndependentSetsSolver::AllMaxIndependentSetsSolver(Parser* parser){
    this->parser = parser;
}

vector<set<int>> AllMaxIndependentSetsSolver::gerateAllMaxIndependentSets(){
    vector<set<int>> maxIdenpendentSets;
    queue.insert({parser->numClausules});
    while(!queue.empty()){
        auto next = queue.begin();
        queue.erase(next);
        for(size_t j = 0; j < parser->cnfGraph.size(); j++){
            for(size_t i = 0; i < parser->cnfGraph[j].size(); i++){
                if(isAdjacent(i,j) && i < j){
                    auto sj = *next;
                    for(auto x : sj){
                        if((size_t)x > j){
                            sj.erase(x);
                        }
                    }
                    for(auto x : parser->cnfGraph[j]){
                        auto xPos = sj.find(x);
                        if(xPos != sj.end()){
                            sj.erase(xPos);
                        }
                    }
                    sj.insert(j);
                    if(isMaximal(sj)){
                        maxIdenpendentSets.push_back(sj);
                    }
                    break;
                }
            }
        }
    }
    maxIdenpendentSets.erase(maxIdenpendentSets.begin());
    return maxIdenpendentSets;
}

bool AllMaxIndependentSetsSolver::isAdjacent(int a, int b){
    return parser->cnfGraph[a].find(b) != parser->cnfGraph[a].end();
}

bool AllMaxIndependentSetsSolver::isMaximal(set<int> potentialSet){
    set<int> visibleClausules;
    for(auto x : potentialSet){
        for(auto y : parser->cnfGraph[x]){
            if(y < parser->numClausules){
                visibleClausules.insert(y);
            }
        }
    }
    return visibleClausules.size() == (size_t)parser->numClausules;
}