#include "twoCnfSolver.hpp"

TwoCnfSolver::TwoCnfSolver(int n, vector<vector<int>> g, vector<vector<int>> gt){
    this->n = n;
    this->g = g;
    this-> gt = gt;
}

void TwoCnfSolver::dfs1(int v) {
    used[v] = true;
    for (int u : g[v]) {
        if (!used[u])
            dfs1(u);
    }
    order.push_back(v);
}

void TwoCnfSolver::dfs2(int v, int cl) {
    comp[v] = cl;
    for (int u : gt[v]) {
        if (comp[u] == -1)
            dfs2(u, cl);
    }
}

bool TwoCnfSolver::solve_2SAT() {
    order.clear();
    used.assign(n, false);
    for (int i = 0; i < n; ++i) {
        if (!used[i])
            dfs1(i);
    }

    comp.assign(n, -1);
    for (int i = 0, j = 0; i < n; ++i) {
        int v = order[n - i - 1];
        if (comp[v] == -1)
            dfs2(v, j++);
    }

    assignment.assign(n / 2, false);
    for (int i = 0; i < n; i += 2) {
        if (comp[i] == comp[i + 1]){
            return false;
        }
        assignment[i / 2] = comp[i] > comp[i + 1];
    }

    return true;
}

vector<int> TwoCnfSolver::getAnswer(){
    vector<int> answer;
    for(size_t i = 0; i < assignment.size(); i++){
        if(assignment[i]){
            answer.push_back(i+1);
        }
        else{
            answer.push_back(-(i+1));
        }
    }
    return answer;
}