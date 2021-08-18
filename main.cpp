#include <iostream>
#include <vector>
#include <fstream>
#include "twoCnfSolver.hpp"

using namespace std;

int main(int argc, char *argv[]){
    ifstream in_file;
    in_file.open(argv[1], ios::in);

    if(argc < 2){
        cout << "Usage: program file_path" << endl;
        return -1;
    }

    if (!in_file) {
		cout << "File not found or not readble" << endl;
        return -2;
	}

    // read preamble
    char line_type;
    
    in_file >> line_type;
    while(line_type != 'p'){
        string line;
        getline(in_file, line);
        in_file >> line_type;
    }
    
    string type;
    int variables, clausules;

    in_file >> type >> variables >> clausules;

    // read clausules
    // vector<vector<int>> g_cnf(variables*2), gt_cnf(variables*2);
    int nodes = variables*2+2;
    vector<vector<int>> g_cnf(nodes), gt_cnf(nodes);
    vector<int> clausules_tmp;
    int num_clausules = 0;

    while(!in_file.eof()){
        int clausule;
        in_file >> clausule;

        if(clausule == 0){
            if(clausules_tmp.size() == 1){
                // int pos1 = clausules_tmp[0] < 0 ? (abs(clausules_tmp[0]+1) << 1 ) + 1 : abs(clausules_tmp[0]-1) << 1;
                int pos1 = clausules_tmp[0] < 0 ? (abs(clausules_tmp[0]) << 1 ) + 1 : abs(clausules_tmp[0]) << 1;

                g_cnf[pos1 ^ 1].push_back(pos1);

                gt_cnf[pos1].push_back(pos1 ^ 1);
            }
            else if(clausules_tmp.size() == 2){
                // int pos1 = clausules_tmp[0] < 0 ? (abs(clausules_tmp[0]+1) << 1 ) + 1 : abs(clausules_tmp[0]-1) << 1;
                // int pos2 = clausules_tmp[1] < 0 ? (abs(clausules_tmp[1]+1) << 1 ) + 1 : abs(clausules_tmp[1]-1) << 1;
                int pos1 = clausules_tmp[0] < 0 ? (abs(clausules_tmp[0]) << 1 ) + 1 : abs(clausules_tmp[0]) << 1;
                int pos2 = clausules_tmp[1] < 0 ? (abs(clausules_tmp[1]) << 1 ) + 1 : abs(clausules_tmp[1]) << 1;

                g_cnf[pos1 ^ 1].push_back(pos2);
                g_cnf[pos2 ^ 1].push_back(pos1);

                gt_cnf[pos2].push_back(pos1 ^ 1);
                gt_cnf[pos1].push_back(pos2 ^ 1);
            }
            else if(clausules_tmp.size() > 2){
                cout << "Wrong number of variables in a clausule (not 2-CNF)" << endl;
                return -3;
            }

            clausules_tmp.clear();
            num_clausules = 0;

            continue;
        }

        num_clausules++;
        clausules_tmp.push_back(clausule);
    }

    // TwoCnfSolver cnf_solver = TwoCnfSolver(variables*2, g_cnf, gt_cnf);
    TwoCnfSolver cnf_solver = TwoCnfSolver(nodes, g_cnf, gt_cnf);
    if(cnf_solver.solve_2SAT()){
        cout << "SAT" << endl;
        cnf_solver.print_answer();
        return 30;
    }
    else{
        cout << "UNSAT" << endl;
        return 20;
    }

}