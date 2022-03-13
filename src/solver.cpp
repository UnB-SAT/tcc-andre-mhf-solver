#include <iostream>
#include "solver.hpp"
#include "utils.hpp"
#include "twoCnfSolver.hpp"

using namespace std;

Solver::Solver(){ }

void Solver::solveSolo(set<int> lits){
    for (auto lit : lits) {
        if (lits.find(lit * -1) != lits.end()) {
            cout << "UNSAT" << endl;
            return;
        }
    }

    Utils::printLine(lits, "SAT");
}

void Solver::solve2Cnf(int numVariables, vector<set<int>> clausules, set<int> soloLiterals){
    int nodes = numVariables*2+2;
    vector<vector<int>> g_cnf(nodes), gt_cnf(nodes);

    for (auto lit : soloLiterals) {
        int pos = lit < 0 ? (abs(lit) << 1 ) + 1 : abs(lit) << 1;
        g_cnf[pos ^ 1].push_back(pos);
        gt_cnf[pos].push_back(pos ^ 1);
    }

    for (auto clausule : clausules) {
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
        solver.print_answer();
    } else {
        cout << "UNSAT" << endl;
    }
}

void Solver::solveHorn(){

}

void Solver::solveMHF(){

}
