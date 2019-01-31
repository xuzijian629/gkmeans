#include "graph.hpp"
#ifndef UNNORMALIZED_H
#define UNNORMALIZED_H
#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <Spectra/SymEigsSolver.h>

// return n * k matrix whose i-th column is the i-th eigenvector
vector<vector<double>> unnormalized_dense(const Graph& g, int K) {
    assert(g.use_adj_mat);
    int n = g.n;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> L(n, n);
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            L(i, j) = -g.adj_mat[i][j];
            sum += g.adj_mat[i][j];
        }
        L(i, i) = sum;
    }
    Spectra::DenseSymMatProd<double> mat(L);
    // read https://spectralib.org/doc/classspectra_1_1symeigssolver for further information
    Spectra::SymEigsSolver<double, Spectra::SMALLEST_ALGE, Spectra::DenseSymMatProd<double>> solve(&mat, K, 2 * K);
    solve.init();
    solve.compute();
    assert(solve.info() == Spectra::SUCCESSFUL);

    vector<vector<double>> ret(n, vector<double>(K));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < K; j++) {
            ret[i][j] = solve.eigenvectors()(i, j);
        }
    }
    return ret;
}

vector<vector<double>> unnormalized_sparse(const Graph& g, int K) {
    assert(!g.use_adj_mat);
    int n = g.n;
    Eigen::SparseMatrix<double> L(n, n);
    vector<Eigen::Triplet<double>> elements;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (auto& p : g.adj[i]) {
            elements.push_back(Eigen::Triplet<double>(i, p.first, -p.second));
            sum += p.second;
        }
        elements.push_back(Eigen::Triplet<double>(i, i, sum));
    }
    L.setFromTriplets(elements.begin(), elements.end());
    Spectra::SparseSymMatProd<double> mat(L);
    Spectra::SymEigsSolver<double, Spectra::SMALLEST_ALGE, Spectra::SparseSymMatProd<double>> solve(&mat, K, 2 * K);
    solve.init();
    solve.compute();
    assert(solve.info() == Spectra::SUCCESSFUL);

    vector<vector<double>> ret(n, vector<double>(K));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < K; j++) {
            ret[i][j] = solve.eigenvectors()(i, j);
        }
    }
    return ret;
}

vector<vector<double>> spectral_unnormalized(const Graph& g, int K) {
    if (g.use_adj_mat) {
        return unnormalized_dense(g, K);
    } else {
        return unnormalized_sparse(g, K);
    }
}

#endif