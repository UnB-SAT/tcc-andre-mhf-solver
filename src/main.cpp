#include <iostream>
#include <vector>
#include <fstream>
#include "parser.hpp"
#include "allMaxIndepSets.hpp"

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
    parser.printCnfGraph();

    AllMaxIndependentSetsSolver solver = AllMaxIndependentSetsSolver(&parser);
    auto a = solver.gerateAllMaxIndependentSets();

    cout << "Solution" << endl;
    for(size_t i = 0; i < a.size(); i++){
        cout << i << ": ";
        for(auto j : a[i]){
            cout << j << " ";
        }
        cout << endl;
    }

    return 0;
}