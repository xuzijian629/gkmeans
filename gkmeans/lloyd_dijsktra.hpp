#include "dijkstra.hpp"
#include <omp.h>
#ifndef LLOYD_DIJKSTRA_H
#define LLOYD_DIJKSTRA_H

vector<int> lloyd_dijkstra(const Graph& g, vector<int> centers, double oversample = 1) {
    int K = centers.size();
    int n = g.n;
    double prev_cost = numeric_limits<double>::infinity();
    vector<int> labels(n);

    int loop = 0;
    while (1) {
        loop++;
        // assign nearest center
        vector<vector<double>> dists(K);
        vector<vector<int>> label_nodes(K);
        #pragma omp parallel for
        for (int i = 0; i < K; i++) {
            dists[i] = dijsktra_sparse(g, centers[i]);
        }
        for (int i = 0; i < n; i++) {
            double min_dist = numeric_limits<double>::infinity();
            #pragma omp parallel for
            for (int j = 0; j < K; j++) {
                if (dists[j][i] < min_dist) {
                    min_dist = dists[j][i];
                    labels[i] = j;
                }
            }
            label_nodes[labels[i]].push_back(i);
        }

        // update center by random choise
        double global_cost = 0;
        random_device rnd;
        #pragma omp parallel for
        for (int i = 0; i < K; i++) {
            double min_cost = numeric_limits<double>::infinity();
            for (int j = 0; j < min(10, int(oversample * label_nodes[i].size())); j++) {
                int center = label_nodes[i][rnd() % label_nodes[i].size()];
                vector<double> dist = dijsktra_sparse(g, center);
                double cost = 0;
                for (int v : label_nodes[i]) {
                    cost += dist[v] * dist[v];
                }
                if (cost < min_cost) {
                    min_cost = cost;
                    centers[i] = center;
                }
            }
            global_cost += min_cost;
        }
        cerr << "loop " << loop << ", cost " << global_cost << endl; 
        if (global_cost >= prev_cost) break;
        prev_cost = global_cost;
    }
    cout << "loop: " << loop << endl;
    cout << "cost: " << prev_cost << endl;
    return labels;
}

#endif