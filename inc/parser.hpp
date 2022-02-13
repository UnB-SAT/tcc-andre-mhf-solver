#ifndef _PARSER_HPP
#define _PARSER_HPP

#include <fstream>
#include <vector>
#include <set>

using namespace std;

class Parser{
public:
    Parser(ifstream* file);
    int getLitPos(int lit);

    int numVariables;
    int numClausules;
    vector<set<int>> cnfGraph;
private:
    void insertIntoCnfGraph(int a, int b);
};

#endif