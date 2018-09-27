import java.io.*;
import java.util.*;

public class F {
    
    public static int mf = 0;
    public static List<List<Edge>> graph;
    
    public static void main (String[] args) throws Exception {
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        
        // Create initial source
        int n = Integer.parseInt(in.readLine());
        graph = new ArrayList<>(n+28);
        for (int i = 0; i < n+28; i++) {
            graph.add(new ArrayList<>());
        }
        
        Edge e;
        for (int i = 1; i <= n; i++) {
            // Source to current
            e = new Edge(0, i, 0, 1);
            graph.get(0).add(e);
            graph.get(i).add(e);
            
            // Get internal layer
            String[] temp = in.readLine().split(" ");
            for (int j: getCareers(temp)) {
                e = new Edge(i, j+n+1, 0, 1);
                graph.get(i).add(e);
                graph.get(j+n+1).add(e);
            }
        }
        
        // Create sink
        int k = Integer.parseInt(in.readLine());
        for (int i = n + 1; i < n + 27; i++) {
            e = new Edge(i, n+27, 0, k);
            graph.get(i).add(e);
            graph.get(n+27).add(e);
        }
        
        EdmondsKarp(0, n+27);
        System.out.println(mf);
    }
    
    static List<Integer> getCareers (String[] careers) {
        List<Integer> counter = new ArrayList<>(26);
        for (int i = 0; i < 26; i++) counter.add(0);
        
        for (String teammember: careers) {
            for (char a : teammember.toCharArray()) {
                int index = a-'A';
                counter.set(index, counter.get(index)+1);
            }
        }
        
        int maximum = Collections.max(counter);
        List<Integer> indices = new ArrayList<>();
        for (int i = 0; i < 26; i++) {
            if (counter.get(i) == maximum) {
                indices.add(i);
            }
        }
        
        return indices;
    }
    
    static int packageInfo(int u, int v) {
        return (u << 16) + v;
    }
    
    static int[] unpackage(int u) {
        int[] info = new int[2];
        info[0] = u >> 16;
        info[1] = u & (1 << 16) - 1;
        return info;
    }
    
    static void EdmondsKarp (int s, int t) throws Exception {
        
        while (true) {
            // Our structures
            PriorityQueue<Integer> q = new PriorityQueue<>();
            q.add(packageInfo(0, s));
            
            int[] p = new int[graph.size()];
            Arrays.setAll(p, temp -> -1);
            
            int[] f = new int[graph.size()];
            f[s] = 1 << 29;
            
            boolean[] fwds = new boolean[graph.size()];
            Edge[] edges = new Edge[graph.size()];
            
            // BFS
            while (q.size() > 0) {
                int[] inf = unpackage(q.poll());
                int dist = inf[0];
                int cur = inf[1];
                
                if (cur == t) break;
                
                for (Edge e: graph.get(cur)) {
                    boolean fwd = (e.u == cur) ? true : false;
                    int next = (fwd) ? e.v : e.u;
                    int capacity = (fwd) ? e.total - e.used : e.used;
                    
                    if (p[next] == -1 && capacity > 0) { // Not been visited yet and has capacity
                        f[next] = Math.min(f[cur], capacity);
                        p[next] = cur;
                        fwds[next] = fwd;
                        edges[next] = e;
                        q.add(packageInfo(dist+1, next));
                    }
                }
            }

            if (p[t] == -1) return; // Couldn't find free path to sink
            
            // Update flows
            int cur = t;
            while (cur != s) { 
                edges[cur].used += (fwds[cur]) ? f[t] : -f[t];
                cur = p[cur];
            }
            mf += f[t];
        }
    }
}

class Edge {
    int u, v, used, total;
    public Edge(int u, int v, int w, int x) {
        this.u = u;
        this.v = v;
        this.used = w;
        this.total = x;
    }
}