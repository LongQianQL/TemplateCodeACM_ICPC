#include <vector>
#include <cstdio>

using namespace std;

struct Standindices;

typedef long long L;
typedef vector<L> VL;
typedef vector<Standindices> VS;

// Custom data class for DP - includes the current index, a pointer to the next stand to buy coffee from, the number of stands and the value of this option (the amount of distance drinking coffee)
struct Standindices {
    int cur, size;
    L value;
    Standindices* next;
    Standindices() : cur(-1), next(NULL), size(0), value(-1LL) {};
};

int main () {
    L standpos, a, b, t, r, gate;
    int n;
    scanf("%lld %lld %lld %lld %lld", &gate, &a, &b, &t, &r);
    scanf("%d", &n);
    
    L cooldist = a * t;
    L drinkdist = b * r + cooldist;
    L withcoffee = drinkdist - cooldist;
    
    VL stands(n);
    VL cools(n);
    for (int i = 0; i < n; i++) {
        scanf("%lld", &standpos);
        stands[i] = standpos;
        cools[i] = standpos + cooldist;
    }
    
    // First two base cases manually completed
    VS memo(n+1, Standindices());  
    if (n > 0) {
        memo[n-1].value = min(gate, stands[n-1] + drinkdist) - cools[n-1];
        memo[n-1].cur = n-1;
        memo[n-1].size++;
    }
    
    if (n > 1) {
        L first = memo[n-1].value + min(stands[n-1] - cools[n-2], withcoffee);
        L none = min(gate, cools[n-2]+withcoffee) - cools[n-2];
        
        if (none >= first) {
            memo[n-2].value = none;
            memo[n-2].size++;
        } else {
            memo[n-2].value = first;
            memo[n-2].next = &memo[n-1];
            memo[n-2].size = memo[n-1].size + 1;
        }
        memo[n-2].cur = n-2;
    }
    
    // Check for the 4 potentially optimal cases, choose the best option at each stand
    int split = n-2;
    int i = n-3;
    while (i >= 0) {
        while (split > i+1 && stands[split] > stands[i] + drinkdist) { split--; }  // Trailing iterator tracking where the last overlapping stand is

        L third = memo[split+1].value + min(stands[split+1] - cools[i], withcoffee);
        L second = memo[split].value + min(stands[split] - cools[i], withcoffee);
        L first = memo[i+1].value + min(stands[i+1] - cools[i], withcoffee);
        L none = min(gate, cools[i]+withcoffee) - cools[i];
        
        if (third >= second && third >= first && third >= none) {
            memo[i].value = third;
            memo[i].next = &memo[split+1];
            memo[i].size = memo[split+1].size + 1;
        } else if (second >= first && second >= none) {
            memo[i].value = second;
            memo[i].next = &memo[split];
            memo[i].size = memo[split].size + 1;
        } else if (first >= none) {
            memo[i].value = first;
            memo[i].next = &memo[i+1];
            memo[i].size = memo[i+1].size + 1;
        } else {
            memo[i].value = none;
            memo[i].size++;
        } 
        memo[i].cur = i;
        i--;
    }
    
    // Print results
    printf("%d\n", memo[0].size);
    Standindices current = memo[0];
    while (current.next != NULL) {
        printf("%d ", current.cur);
        current = *current.next;
    }
    if (n > 0)
        printf("%d", current.cur);
    
    return 0;
}
