#include "dijkstra.hpp"
#ifndef GKMEANSPP_H
#define GKMEANSPP_H

vector<int> sample_centers(const Graph& g, int K) {
    vector<int> centers;
    int n = g.n;
    random_device rnd;
    mt19937 eng(rnd());
    uniform_real_distribution<> ur(0, 1);
    centers.push_back(rnd() % n);
    for (int i = 1; i < K; i++) {
        vector<vector<double>> dists(i);
        for (int j = 0; j < i; j++) {
            dists[j] = dijsktra_sparse(g, centers[j]);
        }
        vector<double> weights(n);
        for (int j = 0; j < n; j++) {
            double min_dist = numeric_limits<double>::infinity();
            for (int k = 0; k < i; k++) {
                min_dist = min(min_dist, dists[k][j]);
            }
            weights[j] = min_dist * min_dist;
        }
        double sum = 0;
        double random_limit = accumulate(weights.begin(), weights.end(), 0.0) * ur(eng);
        for (int j = 0; j < n; j++) {
            sum += weights[j];
            if (sum >= random_limit) {
                centers.push_back(j);
                break;
            }
        }
    }
    return centers;
}

#endif