#include "allMaxIndepSets.hpp"
#include "utils.hpp"
#include <iostream>

AllMaxIndependentSetsSolver::AllMaxIndependentSetsSolver(Parser* parser){
    this->parser = parser;
    queue.clear();
}

vector<set<int>> AllMaxIndependentSetsSolver::gerateAllMaxIndependentSets(){
    vector<set<int>> maxIdenpendentSets;
    set<int> tmp_next;
    for(int i = 0; i < parser->numClausules; i++){
        tmp_next.insert(i);
    }
    queue.insert(tmp_next);
    while(!queue.empty()){
        auto tmp_s = *queue.begin();
        maxIdenpendentSets.push_back(tmp_s);
        for(size_t j = 0; j < parser->cnfGraph.size(); j++){
            for(auto i : tmp_s){
                if(isAdjacent(i,j) && (size_t)i < j){
                    
                    set<int> sj; 
                    for(auto x : tmp_s){
                        if((size_t)x <= j){
                            sj.insert(x);
                        }
                    }

                    auto tj = parser->cnfGraph[j];

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
    maxIdenpendentSets.erase(maxIdenpendentSets.begin());
    return maxIdenpendentSets;
}

bool AllMaxIndependentSetsSolver::isAdjacent(int a, int b){
    return parser->cnfGraph[a].find(b) != parser->cnfGraph[a].end();
}

bool AllMaxIndependentSetsSolver::isMaximal(set<int> potentialSet){
    set<int> visibleClausules;
    for(auto x : potentialSet){
        if (x <= parser->numClausules) {
            visibleClausules.insert(x);
        }
        else {
            for(auto y : parser->cnfGraph[x]){
                if(y < parser->numClausules){
                    visibleClausules.insert(y);
                }
            }
        }
    }
    return visibleClausules.size() == (size_t)parser->numClausules;
}