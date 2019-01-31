#include "dijkstra.hpp"
#include "gkmeanspp.hpp"
#include <omp.h>
#ifndef SCALABLE_KMEANSPP_H
#define SCALABLE_KMEANSPP_H

vector<int> sample_centers_parallel(const Graph& g, int K, int oversample = 1) {
    vector<int> centers;
    int n = g.n;
    random_device rnd;
    mt19937 eng(rnd());
    uniform_real_distribution<> ur(0, 1);
    centers.push_back(rnd() % n);
    for (int i = 0; i < 3; i++) {
        vector<vector<double>> dists(centers.size());
        #pragma omp parallel for
        for (int j = 0; j < centers.size(); j++) {
            dists[j] = dijsktra_sparse(g, centers[j]);
        }
        vector<double> weights(n);
        #pragma omp parallel for
        for (int j = 0; j < n; j++) {
            double min_dist = numeric_limits<double>::infinity();
            for (int k = 0; k < centers.size(); k++) {
                min_dist = min(min_dist, dists[k][j]);
            }
            weights[j] = min_dist * min_dist;
        }
        double sum = accumulate(weights.begin(), weights.end(), 0.0);
        for (int j = 0; j < n; j++) {
            if (ur(eng) < oversample * K * weights[j] / sum) {
                centers.push_back(j);
            }
        }
    }

    int N = centers.size();
    vector<vector<double>> dists(N);
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        dists[i] = dijsktra_sparse(g, centers[i]);
    }

    Graph g2(N);
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            g2.add_edge(i, j, dists[i][centers[j]]);
        }
    }

    vector<int> centers2 = sample_centers(g2, K);
    vector<int> ret;
    for (int i = 0; i < K; i++) {
        ret.push_back(centers[centers2[i]]);
    }
    return ret;
}

#endif