#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

int main(int argc, char *argv[]){
    ifstream file;

    string fileName = argv[1];

    file.open(fileName); // clasp result

    while(1) {
        string line;
        getline(file, line);

        if (line.substr(0, 6) == "c Time") {
            istringstream iss(line);

            string tmp;

            iss >> tmp; // c
            iss >> tmp; // Time
            iss >> tmp; // :
            
            double time;

            iss >> time;

            string cnfFile = fileName.substr(0, fileName.size() - 6);
            string outFile = cnfFile;
            replace(outFile.begin(), outFile.end(), '/', '-');

            string outFileName;

            if (time < 1) {            // 1s
                outFileName = "benchmarks/_1s/" + outFile.substr(13);
            } else if (time < 5) {     // 5s
                outFileName = "benchmarks/_5s/" + outFile.substr(13);
            } else if (time < 30) {    // 30s  
                outFileName = "benchmarks/_30s/" + outFile.substr(13);
            } else if (time < 600) {   // 10m
                outFileName = "benchmarks/10m/" + outFile.substr(13);
            } else if (time < 1800) {  // 30m-
                outFileName = "benchmarks/30m-/" + outFile.substr(13);
            } else {                    // 30m+
                outFileName = "benchmarks/30m+/" + outFile.substr(13);
            }

            ifstream src;
            ofstream dst;
            src.open(cnfFile, ios::in | ios::binary);

            dst.open(outFileName, ios::out | ios::binary | ios::trunc);

            dst << src.rdbuf();

            src.close();
            dst.close();

            return 0;
        }

        if (file.eof()) {
            cerr << "Error" << endl;
            break;
        }
    }

    return 0;
}