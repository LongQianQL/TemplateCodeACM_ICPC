import java.io.*;
import java.util.*;

public class OptimalPaths2d {
    
    static int[] weights;
    static int[] preds;
    static int lava = 0;
    static int n, t;
    
    public static void main (String[] args) {
        // BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        
        // Scanner in = new Scanner(System.in);
        
        // n = in.nextInt();
        // t = in.nextInt();
        
        // weights = new int[n*n+1];
        // preds = new int[n*n+1];
        // for (int i = 1; i <= n*n; i++)
            // weights[i] = in.nextInt();
        
        // for (int i = 0; i < 2*t; i++) {
            // dijkstraMod();
        // }
        
        // System.out.println(lava);
        
        
        Random rand = new Random();
        for (n = 5; n <= 50; n++) {
            for (t = 2; t <= n/2; t++) {
                lava = 0;
                weights = new int[n*n+1];
                preds = new int[n*n+1];
                for (int i = 1; i <= n*n; i++)
                    weights[i] = rand.nextInt(10000)+1;
                
                for (int i = 0; i < 2*t; i++) {
                    dijkstraMod();
                }
                
                System.out.println(n + " " + t);
                for (int i = 1; i < weights.length; i++) {
                    System.out.print(weights[i] + " ");
                    if (i % n == 0)
                        System.out.println();
                }
            }
        }
        
    }
    
    static long pack(int i, int j) {
        return ((long)i << 32) + j;
    }
    
    static int[] unpack(long i) {
        int[] info = new int[2];
        info[0] = (int)(i >> 32);
        info[1] = (int)(i & (1L<<32)-1);
        return info;
    }
    
    
    
    static List<List<Integer>> getNeighbours(int node, int[] thispred, int pred, boolean rec) {
        // if (node < 1) {
            // return new ArrayList<>();
        // }
        
        List<Integer> possible = new ArrayList<>();
        
        possible.add(node+1); // Forward
        possible.add(node-n); // Left
        possible.add(node+n); // Right
        if (node % n != 1) possible.add(node-1); // Back
        
        List<List<Integer>> res = new ArrayList<>();
        for (int next: possible) {
            if (pred == next) continue; // Only recurse backwards
            if (next < 1 || next > n*n) continue;
            if (thispred[next] != 0) continue; // Already explored in this iteration
            if (preds[next] > 0) { // Here we skip and check untaken paths
                List<List<Integer>> res2 = getNeighbours(preds[next], thispred, next, true);
                for (List<Integer> i: res2) {
                    i.add(next);
                    res.add(i);
                }
            } 
            else { // Continue as normal
                List<Integer> thisnext = new ArrayList<>();
                thisnext.add(next);
                thisnext.add(node);
                res.add(thisnext);
            }
        }
        
        return res;
    }
    
    static void dijkstraMod() {
        PriorityQueue<Long> q = new PriorityQueue<>();
        int[] thispred = new int[n*n+1];
        int[] thisdist = new int[n*n+1];
        Arrays.fill(thisdist, 1 << 29);
        long[] jumps = new long[n*n+1];
        
        for (int i = 1; i < weights.length; i += n) {
            if (preds[i] == 0) {
                q.add(pack(weights[i], i));
                thispred[i] = -1;
                thisdist[i] = weights[i];
            }
        }
        
        // BFS with skipping
        int cur = 0, dist = 0;
        while (q.size() > 0) {
            int[] info = unpack(q.poll());
            dist = info[0];
            cur = info[1];
            
            if (cur % n == 0) break; // Reached right edge
            
            for (List<Integer> next: getNeighbours(cur, thispred, -1, false)) {
                int nextnode = next.get(0);
                
                if (dist + weights[nextnode] < thisdist[nextnode]) {
                    thisdist[nextnode] = dist + weights[nextnode];
                    if (next.size() == 2) { // No skipping
                        thispred[nextnode] = cur;
                    } else { // Skips
                        int pred = next.get(1);
                        int jumpsfrom = next.get(2);
                        thispred[nextnode] = pred;
                        jumps[pred] = pack(jumpsfrom, cur);
                    }
                    q.add(pack(dist + weights[nextnode], nextnode));
                }
            }
        }
        
        // Update global variables with this path
        // System.out.println(dist);
        lava += dist;
        // int count = 0;
        while (cur > 0) {
            // System.out.println("Cur: " + cur);
            if (jumps[cur] != 0) {
                int[] inf = unpack(jumps[cur]);
                preds[inf[0]] = inf[1];
                // if (count++ < 10) System.out.println(cur + " " + inf[0] + " " + inf[1] + " " + preds[inf[1]] + " " + preds[inf[0]]);
                cur = inf[1];
            } else {
                preds[cur] = thispred[cur];
                cur = preds[cur];
                // count = 0;
            }
        }
        
        System.out.println("\n");
        for (int i = 1; i < preds.length; i++) {
            System.out.print(preds[i] + "\t");
            if (i % n == 0)
                System.out.println();
        }
    } 
}