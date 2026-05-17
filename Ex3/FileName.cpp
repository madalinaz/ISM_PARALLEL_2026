#include<iostream>
#include<vector>
#include<string>
#include"Methods.h"
using namespace std;

int main() {
	//load balancing
	//open mp
	long n = 1e5;
	benchmank("Sequential", n, sequentialSolution);

	return 0;
}