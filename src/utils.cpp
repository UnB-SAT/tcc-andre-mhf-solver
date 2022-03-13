#include <iostream>
#include "utils.hpp"

void Utils::printMatrix(vector<set<int>> matrix, string title){
    cout << title << endl;
    for(size_t i = 0; i < matrix.size(); i++){
        cout << i << ": ";
        for(auto j : matrix[i]){
            cout << j << " ";
        }
        cout << endl;
    }
}

void Utils::printLine(set<int> line, string title){
    if (title.size()) {
        cout << title << endl;
    }
    for(auto x : line){
        cout << x << " ";
    }
    cout << endl;
}