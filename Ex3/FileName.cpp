#include<iostream>
#include<vector>
#include<string>
#include"Methods.h"
using namespace std;

int main() {
	//load balancing
	//open mp
	long n = 3e5;
	//ONLY SEQUENTIAL
	benchmank("Sequential", n, sequentialSolution);
	benchmank("Sequential with good 0 and 1 (version 1)", n, sequentialSolutionModif);
	benchmank("Sequential with good 0 and 1 (version 2)", n, sequentialSolutionModif);
	benchmank("Sequential optimized (sqrt)", n, sequentialSolutionOptimized);
	//PARALLEL
	benchmank("Parallel with race condition", n, parallelSolution);
	benchmank("Parallel with no race condition (mutex)", n, parallelSolutionMutex);
	return 0;
}