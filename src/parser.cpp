#include <iostream>
#include "parser.hpp"

using namespace std;

Parser::Parser() { }

int Parser::parse(ifstream* file) {
    char line_type;
    
    *file >> line_type;
    while(line_type != 'p'){
        string line;
        getline(*file, line);
        *file >> line_type;
    }
    
    string type;

    *file >> type >> numVariables >> numClausules;

    for(int i = 0; i < numClausules; i++){
        set<int> tmp_s;
        int tmp_i;
        *file >> tmp_i;
        tmp_s.insert(tmp_i);
        int totalPositive = tmp_i > 0 ? 1 : 0;
        while(tmp_i){
            tmp_s.insert(tmp_i);
            *file >> tmp_i;
            if (tmp_i > 0) {
                totalPositive++;
            }
        }
        if(tmp_s.size() == 1){
            soloLiterals.insert(*tmp_s.begin());
        }
        else if(tmp_s.size() == 2){     
            cnfClausules.push_back(tmp_s);

            if (totalPositive < 2) {
                hornClausules.push_back(tmp_s);    
            }
        }
        else{
            if (totalPositive < 2) {
                hornClausules.push_back(tmp_s);
            } else {
                return -1;
            }
        }
    }

    setFunctionType();

    return 0;
}

void Parser::setFunctionType(){
    if (!cnfClausules.size() && !hornClausules.size()) {
        functionType = SOLO;
        return;
    }

    if (cnfClausules.size() + soloLiterals.size() == (size_t)numClausules) {
        functionType = CNF;
        return;
    }

    if (hornClausules.size() + soloLiterals.size() == (size_t)numClausules) {
        functionType = HORN;
        return;
    }

    functionType = MHF;
}