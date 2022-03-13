#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <set>
#include <vector>
#include <string>

using namespace std;

class Utils{
public:
    static void printMatrix(vector<set<int>> matrix, string title);
    static void printLine(set<int> line, string title);
};

#endif