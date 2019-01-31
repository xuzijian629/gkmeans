#ifndef GRAPH_H
#define GRAPH_H
#include <bits/stdc++.h>
using namespace std;

struct Graph {
    int n, m = 0;
    bool use_adj_mat;
    vector<vector<pair<int, double>>> adj;
    vector<vector<double>> adj_mat;

    Graph(int n, bool use_adj_mat = false) : n(n), use_adj_mat(use_adj_mat) {
        if (use_adj_mat) {
            adj_mat = vector<vector<double>>(n, vector<double>(n, numeric_limits<double>::infinity()));
            for (int i = 0; i < n; i++) {
                adj_mat[i][i] = 0;
            }
        } else {
            adj = vector<vector<pair<int, double>>>(n);
        }
    }

    void add_edge(int u, int v, double w = 1) {
        m++;
        assert(0 <= u && u < n && 0 <= v && v < n);
        if (use_adj_mat) {
            adj_mat[u][v] = adj_mat[v][u] = w;
        } else {
            adj[u].emplace_back(v, w);
            adj[v].emplace_back(u, w);
        }
    }

    void read() {
        int M;
        cin >> M;
        for (int i = 0; i < M; i++) {
            int u, v;
            double w;
            cin >> u >> v >> w;
            add_edge(u, v, w);
        }
        assert(m == M);
    }

    void print() {
        cout << n << ' ' << m << '\n';
        if (use_adj_mat) {
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (adj_mat[i][j] != numeric_limits<double>::infinity()) {
                        cout << i << ' ' << j << ' ' << adj_mat[i][j] << '\n';
                    }
                }
            }
        } else {
            set<pair<int, int>> printed;
            for (int i = 0; i < n; i++) {
                for (auto& p : adj[i]) {
                    if (printed.count({min(i, p.first), max(i, p.first)}) == 0) {
                        cout << i << ' ' << p.first << ' ' << p.second << '\n';
                        printed.insert({min(i, p.first), max(i, p.first)});
                    }
                }
            }
        }
    }
};

#endif