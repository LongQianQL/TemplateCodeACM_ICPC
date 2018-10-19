#include <cstdio>

using namespace std;



int power(int n, int pow){
	
	if (pow == 0) return 1;
	if (pow == 1) return n;
	
	if(pow & 1) return n*power(n * n, p/2);
	return power(n*n, p/2)
	
}

