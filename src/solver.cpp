#include <iostream>
#include <iomanip>
#include "solver.hpp"
#include "utils.hpp"
#include "twoCnfSolver.hpp"
#include "hornSolver.hpp"
#include "allMaxIndepSets.hpp"

using namespace std;

Solver::Solver(Parser* parser){
    this->parser = parser;
}

void Solver::solveSolo(){
    for (auto lit : parser->soloLiterals) {
        if (parser->soloLiterals.find(lit * -1) != parser->soloLiterals.end()) {
            returnValue = UNSAT;
            return;
        }
    }

    returnValue = SAT;
    for (auto lit : parser->soloLiterals) {
        answer.push_back(lit);
    }
}

void Solver::solve2Cnf(){
    int nodes = parser->numVariables*2+2;
    vector<vector<int>> g_cnf(nodes), gt_cnf(nodes);

    for (auto lit : parser->soloLiterals) {
        int pos = lit < 0 ? (abs(lit) << 1 ) + 1 : abs(lit) << 1;
        g_cnf[pos ^ 1].push_back(pos);
        gt_cnf[pos].push_back(pos ^ 1);
    }

    for (auto clausule : parser->cnfClausules) {
        vector<int> literals;
        for (auto lit : clausule) {
            literals.push_back(lit);
        }

        int pos1 = literals[0] < 0 ? (abs(literals[0]) << 1 ) + 1 : abs(literals[0]) << 1;
        int pos2 = literals[1] < 0 ? (abs(literals[1]) << 1 ) + 1 : abs(literals[1]) << 1;

        g_cnf[pos1 ^ 1].push_back(pos2);
        g_cnf[pos2 ^ 1].push_back(pos1);

        gt_cnf[pos2].push_back(pos1 ^ 1);
        gt_cnf[pos1].push_back(pos2 ^ 1);
    }

    TwoCnfSolver solver = TwoCnfSolver(nodes, g_cnf, gt_cnf);
    if (solver.solve_2SAT()) {
        returnValue = SAT;
        answer = solver.getAnswer();
    } else {
        returnValue = UNSAT;
    }
}

void Solver::solveHorn(){
    HornSolver solver = HornSolver();

    if (solver.solveHornSat(parser->numVariables, parser->hornClausules, parser->soloLiterals)) {
        returnValue = SAT;
        answer = solver.getAnswer();
    } else {
        returnValue = UNSAT;
    }
}

void Solver::solveMHF(){
    // TODO fix duplications
    int nodes = parser->numVariables*2+2;
    vector<vector<int>> g_cnf(nodes), gt_cnf(nodes);

    for (auto lit : parser->soloLiterals) {
        int pos = lit < 0 ? (abs(lit) << 1 ) + 1 : abs(lit) << 1;
        g_cnf[pos ^ 1].push_back(pos);
        gt_cnf[pos].push_back(pos ^ 1);
    }

    for (auto clausule : parser->cnfClausules) {
        vector<int> literals;
        for (auto lit : clausule) {
            literals.push_back(lit);
        }

        int pos1 = literals[0] < 0 ? (abs(literals[0]) << 1 ) + 1 : abs(literals[0]) << 1;
        int pos2 = literals[1] < 0 ? (abs(literals[1]) << 1 ) + 1 : abs(literals[1]) << 1;

        g_cnf[pos1 ^ 1].push_back(pos2);
        g_cnf[pos2 ^ 1].push_back(pos1);

        gt_cnf[pos2].push_back(pos1 ^ 1);
        gt_cnf[pos1].push_back(pos2 ^ 1);
    }

    TwoCnfSolver _twoCnfSolver = TwoCnfSolver(nodes, g_cnf, gt_cnf);
    if (!_twoCnfSolver.solve_2SAT()) {
        returnValue = UNSAT;
        return;
    }

    HornSolver _hornSolver = HornSolver();

    if (!_hornSolver.solveHornSat(parser->numVariables, parser->hornClausules, parser->soloLiterals)) {
        returnValue = UNSAT;
        return;
    }

    vector<set<int>> cnfGraph;
    cnfGraph.resize(parser->numClausules+1+parser->numVariables*2);

    for(size_t i = 0; i < parser->cnfClausules.size(); i++){
        insertIntoGraph(&cnfGraph, i, parser->numClausules);

        for(auto x : parser->cnfClausules[i]){
            int litPos = getLitPos(x);
            insertIntoGraph(&cnfGraph, litPos, parser->numClausules);
            insertIntoGraph(&cnfGraph, litPos, i);
            auto inverseLitPos = cnfGraph[litPos].find(x*-1);
            if(inverseLitPos != cnfGraph[litPos].end()){
                insertIntoGraph(&cnfGraph, litPos, *inverseLitPos);
            }
        }
    }

    AllMaxIndependentSetsSolver MISSolver = AllMaxIndependentSetsSolver(parser->numVariables, parser->cnfClausules.size(), cnfGraph);

    auto test = MISSolver.gerateAllMaxIndependentSets();

    while (test.size()) {
        HornSolver hornSolver = HornSolver();

        for (auto x : parser->soloLiterals) {
            test.insert(x);
        }

        if (hornSolver.solveHornSat(parser->numVariables, parser->hornClausules, test)) {
            returnValue = SAT;
            answer = hornSolver.getAnswer();
            return;
        }

        test.clear();
        test = MISSolver.gerateAllMaxIndependentSets();
    }
        
    returnValue = UNSAT;
}

void Solver::insertIntoGraph(vector<set<int>>* graph, int a, int b){
    graph->at(a).insert(b);
    graph->at(b).insert(a);
}

int Solver::getLitPos(int lit){
    return parser->numClausules + (lit < 0 ? (abs(lit)<<1) : (lit<<1) - 1);
}

void Solver::generateNewFormula(clock_t startTime, clock_t endTime, string fileName) {
    ofstream outputFile;
    outputFile.open (fileName, ofstream::trunc);
    outputFile << fixed << setprecision(5) << "c time " << ((double)(endTime - startTime)/CLOCKS_PER_SEC) << endl;
    int totalClausules = parser->soloLiterals.size() + parser->hornClausules.size() + parser->cnfClausules.size() + answer.size();
    outputFile << "p cnf " << parser->numVariables << " " << totalClausules << endl;
    for (auto c : parser->cnfClausules) {
        for (auto l : c) {
            outputFile << l << " ";
        }
        outputFile << 0 << endl;
    }
    for (auto c : parser->hornClausules) {
        for (auto l : c) {
            outputFile << l << " ";
        }
        outputFile << 0 << endl;
    }
    for (auto l : parser->soloLiterals) {
        outputFile << l << " 0" << endl;
    }
    for (auto l : answer) {
        outputFile << l << " 0" << endl;
    }
}

