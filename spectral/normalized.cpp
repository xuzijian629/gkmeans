#include "utilities.hpp"
#include "normalized.hpp"
#include "lloyd.hpp"
#include "kmeanspp.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cout.setf(ios::fixed);
    cout.precision(10);
    string s;
    cin >> s;
    assert(freopen(s.c_str(), "r", stdin));
    int K;
    cin >> K;
    vector<int> block_sizes(K);
    for (int i = 0; i < K; i++) {
        cin >> block_sizes[i];
    }
    vector<int> correct_labels = gen_labels_from_block_sizes(block_sizes);
    read_settings();
    int n;
    cin >> n;
    Graph g(n);
    g.read();

    auto start = chrono::system_clock::now();

    cerr << "start spectral decomposition..." << endl;
    vector<vector<double>> decomp = spectral_normalized(g, K);
    assert(decomp.size() == n);

    vector<vector<double>> centers = sample_centers(decomp, K);

    cerr << "start k-means++ clustering..." << endl;
    vector<int> predicted_labels = lloyd(decomp, centers);

    auto finish = chrono::system_clock::now();

    cout << "dataset: " << s << endl;
    cout << "time: " << chrono::duration_cast<chrono::milliseconds>(finish - start).count() << " msec" << endl;
    cout << "accuracy: " << get_accuracy(correct_labels, predicted_labels, K) << endl;
}