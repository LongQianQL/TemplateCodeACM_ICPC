#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> II;
typedef vector<II> VII;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef unordered_set<int> SI;


/*
Observations:

There are up to 100k nodes and roads
If connected, there is one cycle in what is otherwise a tree
There can be more than one edge between two cities

Strategy:
Assign roads and delete from graph at same time. Track which cities still do not have roads assigned to them.
If any pair of cities has two edges between them, assign each city one.
If a city is a leaf, assign that road to them. Do this until no nodes have degree 1.

At this point, there exists a cycle. However it is a simple cycle, so all you need to do is arbitrarily choose one city for a road, and continue.

*/

int assigned = 0;
SI ones;
SI twos;
SI mores;

void updateSets(VVI &graph, int i, bool increasing) {
    
    if (graph[i].size() == 0 && !increasing) {
        ones.erase(i);
    } else if (graph[i].size() == 1) {
        ones.insert(i);
        if (!increasing) twos.erase(i);
    } else if (graph[i].size() == 2) {
        twos.insert(i);
        if (increasing) ones.erase(i);
        else mores.erase(i);
    } else if (graph[i].size() == 3) {
        if (increasing) {
            mores.insert(i);
            twos.erase(i);
        }
    }
}

// Removes last element from graph[cur]
bool remove(VVI &graph, int cur) {
    
    int neighbour = graph[cur].back();
    printf("%d %d\n", cur, neighbour);
    
    graph[cur].pop_back();
    graph[neighbour].erase(find(graph[neighbour].begin(), graph[neighbour].end(), cur));
    
    updateSets(graph, cur, false);
    updateSets(graph, neighbour, false);
    
    assigned++;
    return true;
}

int main () {
    int n, a, b;
    scanf("%d",&n);
    
    VVI graph(n+1, VI());

    for (int i = 0; i < n; i++) {
        scanf("%d %d", &a, &b);
        graph[a].push_back(b);
        graph[b].push_back(a);
        updateSets(graph, a, true);
        updateSets(graph, b, true);
    }
    
    
    while (assigned < n) {
        if (!ones.empty()) {
            remove(graph, *ones.begin());
        } else if (!twos.empty()) {
            remove(graph, *twos.begin());
        } else {
            return 1;
        }
    }
    
    return 0;
}
    