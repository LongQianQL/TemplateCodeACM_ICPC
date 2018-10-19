#include <bits/stdc++.h>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int main() {
    
    while(true) {
        int n, m, q, u, v, w;
        scanf("%d %d %d", &n, &m, &q);
        
        if (n + m + q == 0)
            break;
        
        int mymax = 1 << 28;
        int mymin = -1 << 28;
        
        VVI graph (n, VI(n));
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) graph[i][j] = (i==j)?0:mymax;

        for (int i = 0; i < m; i++) {
            scanf("%d %d %d", &u, &v, &w);
            graph[u][v] = min(graph[u][v], w);
        }
        
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (graph[i][k] != mymax && graph[k][j] != mymax)
                        graph[i][j] = max(mymin, min(graph[i][j], graph[i][k] + graph[k][j]));
                }
            }
        }
        
        for (int k = 0; k < n; k++) {
            if (graph[k][k] < 0)
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        if (graph[i][k] < mymax && graph[k][j] < mymax)
                            graph[i][j] = mymin;
                    }
                }
        }
        
        while(q--) {
            scanf("%d %d", &u, &v);
            if (graph[u][v] == mymax)
                cout << "Impossible" << endl;
            else if (graph[u][v] == mymin)
                cout << "-Infinity" << endl;
            else
                cout << graph[u][v] << endl;
        }
        
        cout << endl;
    }

    return 0;
}