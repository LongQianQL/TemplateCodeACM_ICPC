#include <iostream>
#include <string>

using namespace std;

int r, c, n, r1, c1, r2, c2;
int code = 2;

void floodfill(int** graph, int row, int col, int type) {
    if (row < 0 || row >= r || col < 0 || col >= c || graph[row][col] != type)
        return;
    
    graph[row][col] = code;
    floodfill(graph, row-1, col, type);
    floodfill(graph, row+1, col, type);
    floodfill(graph, row, col-1, type);
    floodfill(graph, row, col+1, type);
}


int main() {
    ios::sync_with_stdio(false);
    cin >> r >> c;
    string temp;
    
    // Set up graph
    int** graph = new int*[r]; // This one always keeps the original data to know if it's binary or decimal area
    int** graphnew = new int*[r]; // This one changes with floodfill
    for (int i = 0; i < r; i++) {
        graph[i] = new int[c];
        graphnew[i] = new int[c];
        cin >> temp;
        for (int j = 0; j < c; j++) {
            graph[i][j] = (int)temp[j]-48;
            graphnew[i][j] = graph[i][j];
        }
    }
    
    // Query graph
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> r1 >> c1 >> r2 >> c2;
        r1--; c1--; r2--; c2--;
        
        // Floodfill the starting point's area if not yet done
        if (graphnew[r1][c1] < 2) {
            floodfill(graphnew, r1, c1, graphnew[r1][c1]);
            code++;
        } 
        
        // If floodfill has connected the two areas, then one can move, otherwise neither can
        if (graphnew[r1][c1] == graphnew[r2][c2]) {
            switch(graph[r1][c1]) {
                case 0: {cout << "binary" << endl; break;}
                case 1: {cout << "decimal" << endl; break;}
            }
        } else {
            cout << "neither" << endl;
        }
    }
    return 0;   
}
