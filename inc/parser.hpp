#ifndef _PARSER_HPP
#define _PARSER_HPP

#define SOLO 0
#define CNF 1
#define HORN 2
#define MHF 3

#include <fstream>
#include <vector>
#include <set>

using namespace std;

class Parser{
public:
    int functionType;

    int numVariables;
    int numClausules;    

    vector<set<int>> cnfClausules;
    vector<set<int>> hornClausules;
    set<int> soloLiterals;

    Parser();

    int parse(ifstream* file);
private:
    void setFunctionType();
};

#endif