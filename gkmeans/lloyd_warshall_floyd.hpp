#include "warshall_floyd.hpp"
#ifndef LLOYD_WARSHALL_FLOYD_H
#define LLOYD_WARSHALL_FLOYD_H

vector<int> lloyd_warshall_floyd(const Graph& g, const vector<vector<double>>& dist, vector<int> centers) {
    int K = centers.size();
    int n = g.n;
    double prev_cost = numeric_limits<double>::infinity();
    vector<int> labels(n);

    int loop = 0;
    while (1) {
        loop++;
        // assign nearst center
        vector<vector<int>> label_nodes(K);
        for (int i = 0; i < n; i++) {
            double min_dist = numeric_limits<double>::infinity();
            for (int j = 0; j < K; j++) {
                if (dist[i][centers[j]] < min_dist) {
                    min_dist = dist[i][centers[j]];
                    labels[i] = j;
                }
            }
            label_nodes[labels[i]].push_back(i);
        }

        // update center
        double global_cost = 0;
        for (int i = 0; i < K; i++) {
            double min_cost = numeric_limits<double>::infinity();
            for (int u : label_nodes[i]) {
                double cost = 0;
                for (int v : label_nodes[i]) {
                    cost += dist[u][v] * dist[u][v];
                }
                if (cost < min_cost) {
                    min_cost = cost;
                    centers[i] = u;
                }
            }
            global_cost += min_cost;
        }
        assert(global_cost <= prev_cost);
        cerr << "loop " << loop << " " << global_cost << endl;
        if (global_cost == prev_cost) break;
        prev_cost = global_cost;
    }
    cout << "loop: " << loop << endl;
    cout << "cost: " << prev_cost << endl;
    return labels;
}

#endif