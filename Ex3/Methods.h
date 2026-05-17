#pragma once

#include<iostream>
#include<omp.h>
using namespace std;

void countPrimes(long lowerLimit, long upperLimit, long& result) {
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j <= i / 2; j++) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}
}


//count of primes using sequential implementation
long sequentialSolution(long size) {
	long noPrimes = 0;
	countPrimes(0, size, noPrimes);
	return noPrimes;
}

int counterBenchmark = 0;

void benchmank(string description, long size, long (*functionPointer)(long)) {
	counterBenchmark++;
	printf("\n\n----------------------%d------------------", counterBenchmark);
	printf("\nTest for %s", description.c_str());
	long startTime = omp_get_wtime();

	double result = functionPointer(size);

	double stopTime = omp_get_wtime();

	printf("\nResult is %d", result);
	printf("\nTime %f seconds", stopTime - startTime);
}




