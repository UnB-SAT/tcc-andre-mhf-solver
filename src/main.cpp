#include <iostream>
#include <vector>
#include <fstream>
#include "parser.hpp"
#include "allMaxIndepSets.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char *argv[]){
    if(argc < 2){
        cout << "Usage: program file_path" << endl;
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
    
    // Utils::printMatrix(&parser.cnfGraph, "CNF Graph");
    // Utils::printMatrix(&parser.cnfClausules, "CNF clausules");
    // Utils::printMatrix(&parser.hornClausules, "Horn clausules");
    // Utils::printLine(&parser.soloLits, "Solo literals");

    AllMaxIndependentSetsSolver solver = AllMaxIndependentSetsSolver(&parser);

    auto matrix = solver.gerateAllMaxIndependentSets();

    Utils::printMatrix(&matrix, "ALL Max Indep Set");

    return 0;
}