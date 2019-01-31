#include "graph.hpp"
#ifndef NORMALIZED_H
#define NORMALIZED_H
#include <Eigen/Eigenvalues>
#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include <Spectra/MatOp/SparseCholesky.h>
#include <Spectra/SymGEigsSolver.h>

// verification by Eigen library
vector<vector<double>> verify(const Graph& g, int K) {
    assert(g.use_adj_mat);
    int n = g.n;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> L(n, n), D(n, n);
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            L(i, j) = -g.adj_mat[i][j];
            sum += g.adj_mat[i][j];
        }
        L(i, i) = sum;
        D(i, i) = sum;
    }
    Eigen::GeneralizedSelfAdjointEigenSolver<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> solve(L, D);
    assert(solve.info() == Eigen::Success);
    
    vector<vector<double>> ret(n, vector<double>(K));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < K; j++) {
            ret[i][j] = solve.eigenvectors()(i, j);
        }
    }
    return ret;
}

vector<vector<double>> normalized_dense(const Graph& g, int K) {
    assert(g.use_adj_mat);
    int n = g.n;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> L(n, n);
    Eigen::SparseMatrix<double> D(n, n);
    vector<Eigen::Triplet<double>> elements;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            L(i, j) = -g.adj_mat[i][j];
            sum += g.adj_mat[i][j];
        }
        L(i, i) = sum;
        elements.push_back(Eigen::Triplet<double>(i, i, sum));
    }
    D.setFromTriplets(elements.begin(), elements.end());

    Spectra::DenseSymMatProd<double> matL(L);
    Spectra::SparseCholesky<double> matD(D);
    Spectra::SymGEigsSolver<double, Spectra::SMALLEST_ALGE, Spectra::DenseSymMatProd<double>, Spectra::SparseCholesky<double>, Spectra::GEIGS_CHOLESKY> solve(&matL, &matD, K, 2 * K);
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

vector<vector<double>> normalized_sparse(const Graph& g, int K) {
    assert(!g.use_adj_mat);
    int n = g.n;
    Eigen::SparseMatrix<double> L(n, n), D(n, n);
    vector<Eigen::Triplet<double>> elementsL, elementsD;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (auto& p : g.adj[i]) {
            elementsL.push_back(Eigen::Triplet<double>(i, p.first, -p.second));
            sum += p.second;
        }
        elementsL.push_back(Eigen::Triplet<double>(i, i, sum));
        elementsD.push_back(Eigen::Triplet<double>(i, i, sum));
    }
    L.setFromTriplets(elementsL.begin(), elementsL.end());
    D.setFromTriplets(elementsD.begin(), elementsD.end());

    Spectra::SparseSymMatProd<double> matL(L);
    Spectra::SparseCholesky<double> matD(D);
    Spectra::SymGEigsSolver<double, Spectra::SMALLEST_ALGE, Spectra::SparseSymMatProd<double>, Spectra::SparseCholesky<double>, Spectra::GEIGS_CHOLESKY> solve(&matL, &matD, K, 2 * K);
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

vector<vector<double>> spectral_normalized(const Graph& g, int K) {
    if (g.use_adj_mat) {
        // return verify(g, K);
        return normalized_dense(g, K);
    } else {
        return normalized_sparse(g, K);
    }
}

#endif
