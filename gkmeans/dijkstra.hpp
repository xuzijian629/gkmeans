#include "graph.hpp"
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

vector<double> dijsktra_sparse(const Graph& g, int s) {
    assert(!g.use_adj_mat);
    vector<double> dist(g.n, numeric_limits<double>::infinity());
    dist[s] = 0;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> que;
    que.push({0, s});

    while (que.size()) {
        auto top = que.top();
        int v = top.second;
        que.pop();

        if (dist[v] < top.first) continue;
        for (auto& u : g.adj[v]) {
            if (dist[u.first] > dist[v] + u.second) {
                dist[u.first] = dist[v] + u.second;
                que.push({dist[u.first], u.first});
            }
        }
    }

    return dist;
}

#endif