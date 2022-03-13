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

void Solver::solve2Cnf(int n, vector<vector<int>> g, vector<vector<int>> gt){
    TwoCnfSolver solver = TwoCnfSolver(n, g, gt);
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
