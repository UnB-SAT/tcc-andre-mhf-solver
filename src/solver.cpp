#include <iostream>
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
            cout << "UNSAT" << endl;
            return;
        }
    }

    Utils::printLine(parser->soloLiterals, "SAT");
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
        cout << "SAT" << endl;
        solver.printAnswer();
    } else {
        cout << "UNSAT" << endl;
    }
}

void Solver::solveHorn(){
    HornSolver solver = HornSolver();

    if (solver.solveHornSat(parser->numVariables, parser->hornClausules, parser->soloLiterals)) {
        cout << "SAT" << endl;
        solver.printAnswer();
    } else {
        cout << "UNSAT" << endl;
    }
}

void Solver::solveMHF(){
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

    AllMaxIndependentSetsSolver MISSolver = AllMaxIndependentSetsSolver(parser->numVariables, parser->numClausules, cnfGraph);
    auto tests = MISSolver.gerateAllMaxIndependentSets();

    for (auto test : tests) {
        HornSolver hornSolver = HornSolver();
        if (hornSolver.solveHornSat(parser->numVariables, parser->hornClausules, test)) {
            cout << "SAT" << endl;
            hornSolver.printAnswer();
        }
    }
        
    cout << "UNSAT" << endl;
}

void Solver::insertIntoGraph(vector<set<int>>* graph, int a, int b){
    graph->at(a).insert(b);
    graph->at(b).insert(a);
}

int Solver::getLitPos(int lit){
    return parser->numClausules + (lit < 0 ? (abs(lit)<<1) : (lit<<1) - 1);
}