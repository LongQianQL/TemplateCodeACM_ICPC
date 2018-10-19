#include <bits/stdc++.h>

using namespace std;

int main() {
    int tests, temp, diff;
    
    while (scanf("%d: ", &tests), tests) {
        
        vector<int> nums;
        while (tests--) {
            scanf("%d", &temp);
            nums.push_back(temp);
        }
        
        bool anti = true;
        for (int i = 0; i < nums.size(); i++) {
            bitset<20000> diffs;
            for (int j = i+1; j < nums.size(); j++) {
                diff = nums[i] - nums[j];
                if (!(diff & 1)) {
                    if (diffs[diff/2+10000]) {
                        anti = false;
                        break;
                    }
                }
                diffs[diff+10000] = 1;
            }
            if (!anti)
                break;
        }
        
        if (anti) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
        
    }
    
    return 0;
}
