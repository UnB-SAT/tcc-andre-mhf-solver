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
        if(tmp_s.size() == 2){
            for(auto x : tmp_s){
                int litPos = getLitPos(x);
                insertIntoCnfGraph(litPos, numClausules);
                insertIntoCnfGraph(litPos, i);
                auto inverserLitPos = cnfGraph[litPos].find(x*-1);
                if(inverserLitPos != cnfGraph[litPos].end()){
                    insertIntoCnfGraph(litPos, *inverserLitPos);
                }
            }
        }
    }
}

void Parser::insertIntoCnfGraph(int a, int b){
    cnfGraph[a].insert(b);
    cnfGraph[b].insert(a);
}

int Parser::getLitPos(int lit){
    return numClausules + (lit < 0 ? (abs(lit)<<1) : (lit<<1) - 1);
}

void Parser::printCnfGraph(){
    cout << "CNF Graph:" << endl;
    for(size_t i = 0; i < cnfGraph.size(); i++){
        cout << i << ": ";
        for(auto j : cnfGraph[i]){
            cout << j << " ";
        }
        cout << endl;
    }
}