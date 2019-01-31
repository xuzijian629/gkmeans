#include "graph.hpp"
#ifndef WARSHALL_FLOYD_H
#define WARSHALL_FLOYD_H

vector<vector<double>> warshall_floyd(const Graph& g) {
    int n = g.n;
    assert(g.use_adj_mat);
    vector<vector<double>> dist(g.adj_mat);
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    return dist;
}

#endif