#include <iostream>
#include <vector>
#include <fstream>
#include "parser.hpp"
#include "allMaxIndepSets.hpp"
#include "utils.hpp"
#include "solver.hpp"

#define DEBUG 0

using namespace std;

int main(int argc, char *argv[]){
    if(argc < 2){
        cout << "Invalid args" << endl;
        cout << "Usage: " << argv[0] << " mhf_file_path" << endl;
        return -1;
    }

    ifstream in_file;
    in_file.open(argv[1], ios::in);

    if (!in_file) {
		cout << "File not found or not readble" << endl;
        return -2;
	}

    Parser parser = Parser(&in_file);
    in_file.close();
    
    if (DEBUG) {
        Utils::printMatrix(parser.cnfGraph, "CNF Graph");
        Utils::printMatrix(parser.cnfClausules, "CNF clausules");
        Utils::printMatrix(parser.hornClausules, "Horn clausules");
        Utils::printLine(parser.soloLits, "Solo literals");
        cout << parser.functionType << endl;
    }

    Solver solver = Solver();

    switch (parser.functionType) {
        case SOLO:
            solver.solveSolo(parser.soloLits);
            break;
        case CNF:
            solver.solve2Cnf(parser.numVariables, parser.cnfClausules, parser.soloLits);
            break;
        case HORN:
            solver.solveHorn(parser.numVariables, parser.hornClausules, parser.soloLits);
            break;
        case MHF:
            //solver.solveMHF();
            break;
        default:
            cout << "Invalid function type" << endl;
            return -3;
    }

    // AllMaxIndependentSetsSolver solver = AllMaxIndependentSetsSolver(&parser);

    // auto matrix = ;

    // Utils::printMatrix(&solver.gerateAllMaxIndependentSets(), "ALL Max Indep Set");

    return 0;
}