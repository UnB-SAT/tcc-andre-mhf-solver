#include <iostream>
#include "parser.hpp"

using namespace std;

Parser::Parser(ifstream* file){
    char line_type;
    
    *file >> line_type;
    while(line_type != 'p'){
        string line;
        getline(*file, line);
        *file >> line_type;
    }
    
    string type;

    *file >> type >> numVariables >> numClausules;

    cnfGraph.resize(numClausules+1+numVariables*2);

    for(int i = 0; i < numClausules; i++){
        insertIntoCnfGraph(i, numClausules);
        set<int> tmp_s;
        int tmp_i;
        *file >> tmp_i;
        tmp_s.insert(tmp_i);
        while(tmp_i){
            tmp_s.insert(tmp_i);
            *file >> tmp_i;
        }
        if(tmp_s.size() == 1){
            soloLits.insert(*tmp_s.begin());
        }
        else if(tmp_s.size() == 2){
            for(auto x : tmp_s){
                int litPos = getLitPos(x);
                insertIntoCnfGraph(litPos, numClausules);
                insertIntoCnfGraph(litPos, i);
                auto inverserLitPos = cnfGraph[litPos].find(x*-1);
                if(inverserLitPos != cnfGraph[litPos].end()){
                    insertIntoCnfGraph(litPos, *inverserLitPos);
                }
            }
            cnfClausules.push_back(tmp_s);

            int totalPositive = 0;
            for (auto tmp : tmp_s) {
                if (tmp > 0) {
                    totalPositive++;
                }
            }
            if (totalPositive < 2) {
                hornClausules.push_back(tmp_s);    
            }
        }
        else{
            hornClausules.push_back(tmp_s);
        }
    }

    setFunctionType();
}

void Parser::insertIntoCnfGraph(int a, int b){
    cnfGraph[a].insert(b);
    cnfGraph[b].insert(a);
}

int Parser::getLitPos(int lit){
    return numClausules + (lit < 0 ? (abs(lit)<<1) : (lit<<1) - 1);
}

void Parser::setFunctionType(){
    if (!cnfClausules.size() && !hornClausules.size()) {
        functionType = SOLO;
        return;
    }

    if (cnfClausules.size() + soloLits.size() == (size_t)numClausules) {
        functionType = CNF;
        return;
    }

    if (hornClausules.size() + soloLits.size() == (size_t)numClausules) {
        functionType = HORN;
        return;
    }

    functionType = MHF;
}