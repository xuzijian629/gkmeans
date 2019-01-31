#include "gkmeans/graph.hpp"

struct UnionFind {
    int n, cnt;
    vector<int> par, rank, sz;

    UnionFind(int n) : n(n), cnt(n), par(n), rank(n), sz(n, 1) {
        iota(par.begin(), par.end(), 0);
    }

    int find(int x) {
        if (x == par[x]) return x;
        return par[x] = find(par[x]);
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int size(int x) {
        return sz[find(x)];
    }

    void unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return;
        if (rank[x] < rank[y]) {
            par[x] = y;
            sz[y] += sz[x];
        } else {
            par[y] = x;
            sz[x] += sz[y];
            if (rank[x] == rank[y]) {
                rank[x]++;
            }
        }
        cnt--;
    }
};

// 変数はブロックの個数、各ブロックのサイズ、ブロッグ内密度、ブロック間密度、ブロック内の辺の重み、ブロック間の辺の重み
Graph gen_block_model(int block_num, vector<int> block_sizes, double in_density, double out_density, double in_w, double out_w, bool use_adj_mat = false) {
    assert(block_num == block_sizes.size());
    int n = accumulate(block_sizes.begin(), block_sizes.end(), 0);
    vector<int> label(n);
    {
        int i = 0;
        for (int j = 0; j < block_num; j++) {
            for (int k = 0; k < block_sizes[j]; k++) {
                label[i++] = j;
            }
        }
    }
    Graph g(n, use_adj_mat);
    UnionFind uf(n);
    random_device rnd;
    mt19937 eng(rnd());
    uniform_real_distribution<> ur(0, 1);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (label[i] == label[j]) {
                if (ur(eng) < in_density) {
                    g.add_edge(i, j, in_w);
                    uf.unite(i, j);
                }
            } else {
                if (ur(eng) < out_density) {
                    g.add_edge(i, j, out_w);
                    uf.unite(i, j);
                }
            }
        }
    }
    // assert connected
    assert(uf.cnt == 1);
    return g;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cout.setf(ios::fixed);
    cout.precision(10);
    int K;
    cin >> K;
    vector<int> block_sizes(K);
    for (int i = 0; i < K; i++) {
        cin >> block_sizes[i];
    }
    double in_density, out_density, in_w, out_w;
    cin >> in_density >> out_density >> in_w >> out_w;
    string id = "";
    for (int s : block_sizes) {
        id += "_" + to_string(s);
    }
    assert(freopen(("data/sbm" + id).substr(0, min(8 + int(id.size()), 100)).c_str(), "w", stdout));
    cout << block_sizes.size() << '\n';
    for (int s : block_sizes) {
        cout << s << ' ';
    }
    cout << '\n';
    cout << in_density << ' ' << out_density << ' ' << in_w << ' ' << out_w << '\n';
    cerr << "start generating graph " + id + "..." << endl;
    Graph g = gen_block_model(block_sizes.size(), block_sizes, in_density, out_density, in_w, out_w);
    cerr << "printing graph..." << endl;
    g.print();
}
