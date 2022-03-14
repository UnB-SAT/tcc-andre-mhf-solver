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

            // TODO: 2 clausules could be also Horn
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
    if (hornClausules.size()) {
        if (cnfClausules.size()) {
            functionType = MHF;
        } else {
            functionType = HORN;
        }
    } else {
        if (cnfClausules.size()) {
            functionType = CNF;
        } else {
            functionType = SOLO;
        }
    }
}