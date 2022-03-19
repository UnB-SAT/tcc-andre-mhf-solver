#include <iostream>
#include <vector>
#include <fstream>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>
#include <string>
#include "parser.hpp"
#include "allMaxIndepSets.hpp"
#include "utils.hpp"
#include "solver.hpp"

using namespace std;

clock_t startTime;
clock_t endTime;

void sigAlarmHandler(int sigNum){
    if (sigNum == SIGALRM) {
        cout << "s UNKNOW" << endl;
        endTime = clock();
        exit(UNKNOW);
    }
}

void beforeExit() {
    cout << fixed << setprecision(5) << "c time " << ((double)(endTime - startTime)/CLOCKS_PER_SEC) << endl;
}

int main(int argc, char *argv[]){
    if(argc < 4){
        cout << "Invalid args" << endl;
        cout << "Usage: " << argv[0] << " mhf_file_path time_limit output_variable_name" << endl;
        return -1;
    }

    atexit(beforeExit);

    signal(SIGALRM, sigAlarmHandler);
    alarm(atoi(argv[2]));

    startTime = clock();

    ifstream in_file;
    in_file.open(argv[1], ios::in);

    if (!in_file) {
		cout << "File not found or not readble" << endl;
        return -2;
	}

    Parser parser = Parser();
    auto parseError = parser.parse(&in_file);
    in_file.close();

    if (parseError) {
        cout << "Not a MHF" << endl;
        return -3;
    }

    Solver solver = Solver(&parser);

    switch (parser.functionType) {
        case SOLO:
            solver.solveSolo();
            break;
        case CNF:
            solver.solve2Cnf();
            break;
        case HORN:
            solver.solveHorn();
            break;
        case MHF:
            solver.solveMHF();
            break;
        default:
            cout << "Invalid function type" << endl;
            return -4;
    }

    if (solver.returnValue == SAT) {
        cout << "s SATISFIABLE" << endl;
        cout << "v ";
        for (auto v : solver.answer) {
            cout << v << " ";
        }
        cout << "0" << endl;
        endTime = clock();
        solver.generateNewFormula(startTime, endTime, argv[3]);
    } else {
        cout << "s UNSATISFIABLE" << endl;
        endTime = clock();
    }

    exit(solver.returnValue);
}