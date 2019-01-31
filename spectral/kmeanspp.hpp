#include <bits/stdc++.h>
using namespace std;
#ifndef KMEANSPP_H
#define KMEANSPP_H

vector<vector<double>> sample_centers(const vector<vector<double>>& points, int K) {
    vector<int> centers;
    int n = points.size();
    int D = points.front().size();
    random_device rnd;
    mt19937 eng(rnd());
    uniform_real_distribution<> ur(0, 1);
    centers.push_back(rnd() % n);
    for (int i = 1; i < K; i++) {
        vector<double> min_dists(n, numeric_limits<double>::infinity());
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < i; k++) {
                double sum = 0;
                for (int d = 0; d < D; d++) {
                    sum += pow(points[centers[k]][d] - points[j][d], 2);
                }
                min_dists[j] = min(min_dists[j], sqrt(sum));
            }
        }
        double sum = 0;
        double random_limit = accumulate(min_dists.begin(), min_dists.end(), 0.0) * ur(eng);
        for (int j = 0; j < n; j++) {
            sum += min_dists[j];
            if (sum >= random_limit) {
                centers.push_back(j);
                break;
            }
        }
    }
    vector<vector<double>> ret;
    for (int i = 0; i < K; i++) {
        ret.push_back(points[centers[i]]);
    }
    return ret;
}

#endif