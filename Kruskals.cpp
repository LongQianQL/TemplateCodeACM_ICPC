#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> II;
typedef pair<int, II> III;
typedef vector<III> VIII;
typedef vector<int> VI;

class UnionFind {
    private: VI p, rank;
    public:
    UnionFind(int n) {
        p.assign(n, 0);
        rank.assign(n, 0);
        for (int i = 0; i < n; i++) {
            p[i] = i;
        }
    }
    int findSet(int i) {
        return (p[i]==i) ? i : findSet(p[i]);
    }
    bool isSameSet(int i, int j) {
        return findSet(i) == findSet(j);
    }
    void unionSet(int i, int j) {
        int pi = findSet(i), pj = findSet(j);
        if (pi == pj) return;
        
        if (rank[pi] > rank[pj]) {
            p[pj] = pi;
        } else {
            p[pi] = p[pj];
            if (rank[pj] == rank[pi]) {
                rank[pj]++;
            } 
        }
    }
};

int main() {
    int t, m, c, u, v, w;
    scanf("%d", &t);
    while (t-- && scanf("%d%d", &m, &c)) {
        VIII edges(c*(c-1)/2);
        for (int i = 0; i < c*(c-1)/2; i++) {
            scanf("%d%d%d", &u, &v, &w);
            edges[i] = III(w, II(u,v));
        }
        
        m -= c;
        sort(edges.begin(), edges.end());
        UnionFind uf(c);
        
        for (III edge: edges) {
            int w = edge.first;
            int u = edge.second.first;
            int v = edge.second.second;
            if (!uf.isSameSet(u, v)) {
                uf.unionSet(u, v);
                m -= w;
            }
        }
        
        if (m < 0) {
            cout << "no" << endl;
        } else {
            cout << "yes" << endl;
        }
        
    }
    
    
    
    
    return 0;
}