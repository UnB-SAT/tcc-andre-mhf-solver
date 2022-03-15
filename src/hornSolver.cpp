#include <cmath>
#include <iostream>
#include "hornSolver.hpp"
#include "utils.hpp"

using namespace std;

HornSolver::HornSolver(){ }

bool HornSolver::solveHornSat(int numVariables, vector<set<int>> _clausules, set<int> initialValues = {}){
    clausules = _clausules;
    this->numVariables = numVariables;

    for (auto value : initialValues) {
        if (setValue(value)){
            return false;
        }
    }

    size_t i = 0;
    while (i < clausules.size()) {
        if (clausules[i].size() == 1 && values.find(abs(*clausules[i].begin())) == values.end()) {
            if (setValue(*clausules[i].begin())) {
                return false;
            }
            i = 0;
        } else {
            i++;
        }
    }

    for (auto c : clausules) {
        if (*c.rend() > 0) {
            if (setValue(*c.rend())) {
                return false;
            }
        }
    }

    for (auto c : clausules) {
        if (!c.size()) {
            return false;
        }
    }

    return true;
}

int HornSolver::setValue(int value){
    auto absValue = abs(value);
    auto valuePos = values.find(absValue);
    if (valuePos == values.end()) {
        values.insert(pair<int, bool>(absValue, value > 0 ? true : false));
        for (size_t i = 0; i < clausules.size(); i++) {
            auto negatePos = clausules[i].find(value * -1);
            if (negatePos != clausules[i].end()) {
                if (clausules[i].size() <= 1) {
                    return -1;
                }
                clausules[i].erase(negatePos);
            }
        }
    } else {
        if (valuePos->second == true && value < 0) {
            return -1;
        } else if(valuePos->second == false && value > 0) {
            return -1;
        }
    }
    return 0;
}

void HornSolver::printAnswer() {
    for (auto i = 1; i <= numVariables; i++) {
        auto valuePos = values.find(i);
        if (valuePos != values.end()) {
            if (valuePos->second) {
                cout << i << " ";
                continue;
            }
        }
        cout << (i * -1) << " ";
    }
    cout << endl;
}