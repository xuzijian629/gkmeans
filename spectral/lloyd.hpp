#include <bits/stdc++.h>
using namespace std;

#ifndef LLOYD
#define LLOYD

vector<int> lloyd(const vector<vector<double>>& points, vector<vector<double>> centers) {
    int n = points.size();
    int K = centers.size();
    assert(points.front().size() == centers.front().size());
    int D = points.front().size();
    vector<int> labels(n);
    double prev_cost = numeric_limits<double>::infinity();
    while (1) {
        double global_cost = 0;
        for (int i = 0; i < n; i++) {
            double min_dist = numeric_limits<double>::infinity();
            for (int j = 0; j < K; j++) {
                double dist = 0;
                for (int d = 0; d < D; d++) {
                    dist += pow(points[i][d] - centers[j][d], 2);
                }
                dist = sqrt(dist);
                if (dist < min_dist) {
                    min_dist = dist;
                    labels[i] = j;
                }
            }
            global_cost += min_dist;
        }

        centers = vector<vector<double>>(K, vector<double>(D, 0));
        vector<int> cnt(K);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < D; j++) {
                centers[labels[i]][j] += points[i][j];
            }
            cnt[labels[i]]++;
        }
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < D; j++) {
                centers[i][j] /= cnt[i];
            }
        }

        if (global_cost < prev_cost) {
            prev_cost = global_cost;
        } else {
            break;
        }
    }
    return labels;
}

#endif