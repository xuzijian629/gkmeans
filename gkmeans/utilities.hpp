#include <bits/stdc++.h>
using namespace std;
#ifndef UTILITIES_H
#define UTILITIES_H

vector<int> gen_labels_from_block_sizes(const vector<int>& block_sizes) {
    int n = accumulate(block_sizes.begin(), block_sizes.end(), 0);
    vector<int> ret(n);
    int i = 0;
    for (int j = 0; j < block_sizes.size(); j++) {
        for (int k = 0; k < block_sizes[j]; k++) {
            ret[i++] = j;
        }
    }
    return ret;
}

void read_settings() {
    // just read through
    double in_density, out_density, in_w, out_w;
    cin >> in_density >> out_density >> in_w >> out_w;
}

class Hungarian {
    int n, p, q;
    vector<vector<int>> mat;
    vector<int> fx, fy, x, y;
    const int INF = __INT_MAX__;

public:
    Hungarian(const vector<vector<int>>& mat) : mat(mat) {
        n = mat.size();
        fx.assign(n, INF);
        fy.assign(n, 0);
        x.assign(n, -1);
        y.assign(n, -1);
    } 

    int run() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fx[i] = max(fx[i], mat[i][j]);
            }
        }

        for (int i = 0; i < n;) {
            vector<int> t(n, -1), s(n + 1, i);
            for (p = q = 0; p <= q && x[i] < 0; p++) {
                for (int k = s[p], j = 0; j < n && x[i] < 0; j++) {
                    if (fx[k] + fy[j] == mat[k][j] && t[j] < 0) {
                        s[++q] = y[j];
                        t[j] = k;
                        if (s[q] < 0) {
                            for (p = j; p >= 0; j = p) {
                                y[j] = k = t[j];
                                p = x[k];
                                x[k] = j;
                            }
                        }
                    }
                }
            }
            if (x[i] < 0) {
                int d = INF;
                for (int k = 0; k <= q; k++) {
                    for (int j = 0; j < n; j++) {
                        if (t[j] < 0) {
                            d = min(d, fx[s[k]] + fy[j] - mat[s[k]][j]);
                        }
                    }
                }
                for (int j = 0; j < n; j++) {
                    fy[j] += (t[j] < 0 ? 0 : d);
                }
                for (int k = 0; k <= q; k++) {
                    fx[s[k]] -= d;
                }
            } else {
                i++;
            }
        }
        int ret = 0;
        for (int i = 0; i < n; i++) {
            ret += mat[i][x[i]];
        }
        return ret;
    }

    int match_y(int k) {
        return x[k];
    }

    int match_x(int k) {
        return y[k];
    }
};

double get_accuracy(const vector<int>& as, const vector<int>& bs, int K) {
    assert(as.size() == bs.size());
    int n = as.size();
    vector<vector<int>> al(K), bl(K);
    for (int i = 0; i < n; i++) {
        al[as[i]].push_back(i);
        bl[bs[i]].push_back(i);
    }
    vector<vector<int>> mat(K, vector<int>(K));
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            vector<int> isec;
            set_intersection(al[i].begin(), al[i].end(), bl[j].begin(), bl[j].end(), back_inserter(isec));
            mat[i][j] = isec.size();
        }
    }
    Hungarian h(mat);
    return double(h.run()) / n;
}

#endif