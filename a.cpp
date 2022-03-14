#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

vector<set<int>> clausules;
map<int, bool> values;

bool solveHornSat(int numVariables, vector<set<int>> clausules);
int setValue(int value);
void printAnswer(int numVariables);

int main()
{
    string unsed_p;
    string unsed_cnf;
    int v;
    int c;
    cin >> unsed_p >> unsed_cnf >> v >> c;
    cin.ignore();
    for (int i = 0; i < c; i++)
    {
        set<int> tmp_s;
        int tmp_i;
        cin >> tmp_i;
        tmp_s.insert(tmp_i);
        while (tmp_i)
        {
            tmp_s.insert(tmp_i);
            cin >> tmp_i;
        }
        clausules.push_back(tmp_s);
    }

    bool isSat = solveHornSat(v, clausules);

    if (isSat)
    {
        cout << "s SATISFIABLE" << endl;
        printAnswer(v);
        return 0;
    }

    cout << "s UNSATISFIABLE" << endl;
    return 0;
}

bool solveHornSat(int numVariables, vector<set<int>> clausules)
{
    size_t i = 0;
    while (i < clausules.size())
    {
        if (clausules[i].size() == 1 && values.find(i) != values.end())
        {
            if (setValue(*clausules[i].begin()))
            {
                return false;
            }
            i = 0;
        }
        else
        {
            i++;
        }
    }
    cout << 3 << endl;

    for (auto c : clausules)
    {
        if (*c.rend() > 0)
        {
            if (setValue(*c.rend()))
            {
                return false;
            }
        }
    }
    cout << 2 << endl;

    for (auto c : clausules)
    {
        if (!c.size())
        {
            return false;
        }
    }

    cout << 1 << endl;

    return true;
}

int setValue(int value)
{
    auto absValue = abs(value);
    auto valuePos = values.find(absValue);
    if (valuePos == values.end())
    {
        values.insert(pair<int, bool>(absValue, value > 0 ? true : false));
        for (size_t i = 0; i < clausules.size(); i++)
        {
            auto negatePos = clausules[i].find(value * -1);
            if (negatePos != clausules[i].end())
            {
                if (clausules[i].size() <= 1)
                {
                    return -1;
                }
                clausules[i].erase(negatePos);
            }
        }
    }
    else
    {
        if (valuePos->second == true && value < 0)
        {
            return -1;
        }
        else if (valuePos->second == false && value > 0)
        {
            return -1;
        }
    }
    return 0;
}

void printAnswer(int numVariables)
{
    for (auto i = 1; i <= numVariables; i++)
    {
        auto valuePos = values.find(i);
        if (valuePos != values.end())
        {
            if (valuePos->second)
            {
                cout << i << " ";
                continue;
            }
        }
        cout << (i * -1) << " ";
    }
    cout << "0" << endl;
}