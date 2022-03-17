#include <iostream>
#include <vector>
#include <fstream>
#include <signal.h>
#include <unistd.h>
#include "parser.hpp"
#include "allMaxIndepSets.hpp"
#include "utils.hpp"
#include "solver.hpp"

#define ALARMTIME 60*30

using namespace std;

// Repo de formular pegar 3 de cada dominio (começo meio fim)
// Impar -> unsat
// Par -> sat

// Colocar saida padrao o site

void sigAlarmHandler(int sigNum){
    if (sigNum == SIGALRM) {
        cout << "UNKNOW" << endl;
        exit(UNKNOW);
    }
}

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

    Parser parser = Parser();
    auto parseError = parser.parse(&in_file);
    in_file.close();

    if (parseError) {
        cout << "Not a MHF" << endl;
        return -3;
    }

    signal(SIGALRM, sigAlarmHandler);
    alarm(ALARMTIME);

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

    return 0;
}