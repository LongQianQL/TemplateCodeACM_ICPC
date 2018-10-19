#include <cstdio>
#include <vector>
#include <queue>
#include <climits>
#include <iostream>

using namespace std;

struct Edge {
    int u, v, c, w, used;
    Edge(int u, int v, int c, int w, int used) : u(u), v(v), c(c), w(w), used(used) {}
};

typedef vector<Edge*> ve;
typedef vector<ve> vve;
typedef vector<int> vi;
typedef priority_queue<pair<int, int> > pq;

int s, t, mf = 0;
long long mincost = 0LL;

void dijkstra(ve &parent, vve &graph, vi &cost) {
    pq q;
    q.push(make_pair(0, s));
    cost[s] = 0;
    
    while (!q.empty()) {
        pair<int, int> node = q.top(); q.pop();
        if (node.second == t) break;
        
        for (int i = 0; i < graph[node.second].size(); i++) {
            Edge* e = graph[node.second][i];
            if (node.second == e->v) { // Residual edge
                int c = cost[e->v] - e->w;
                if (cost[e->u] > c && e->used > 0) {
                    parent[e->u] = e; cost[e->u] = c;
                    q.push(make_pair(-c, e->u));
                }
            }
            else { // Normal edge
                int c = cost[e->u] + e->w;
                if (cost[e->v] > c && e->c > e->used) {
                    parent[e->v] = e; cost[e->v] = c;
                    q.push(make_pair(-c, e->v));
                }
            }
        }
    }
}

int adjust(ve &parent, vve &graph, vi &cost) {
    int f = INT_MAX;
    
    // Find minimum flow on path
    int node = t;
    while (node != s) {
        Edge* e = parent[node];
        if (e->v == node) { // normal direction
            if (e->c - e->used < f) f = e->c - e->used;
            node = e->u;
        } else { // residual direction
            if (e->used < f) f = e->used;
            node = e->v;
        }
    }
    
    // Add flow to path
    mf += f;
    node = t;
    while (node != s) {
        Edge* e = parent[node];
        if (e->v == node) { // normal direction
            e->used += f;
            mincost += (long long)f*e->w;
            node = e->u;
        } else { // residual direction
            e->used -= f;
            mincost -= (long long)f*e->w;
            node = e->v;
        }
    }
    
    return f;
}


int main() {
    int n, m, u, v, c, w;
    scanf("%d %d %d %d", &n, &m, &s, &t);
    
    vve graph(n);
    
    for(int i = 0; i < n; i++) {
        ve list;
        graph.push_back(list);
    } 
    
    for(int i = 0; i < m; i++) {
        scanf("%d %d %d %d", &u, &v, &c, &w);
        Edge* e = new Edge(u, v, c, w, 0);
        graph[e->u].push_back(e);
        graph[e->v].push_back(e);
    }
    
    while (true) {
        ve parent(n);
        vi cost(n, INT_MAX);
        dijkstra(parent, graph, cost);
        if (cost[t] == INT_MAX) break;
        if (adjust(parent, graph, cost) == 0) break;
    }
    
    cout << mf << ' ' << mincost;
    
    return 0;
}