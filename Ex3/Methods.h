#pragma once

#include<iostream>
#include<omp.h>
#include<thread>
#include<vector>
#include<mutex>
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

//because of the 0 and 1
void countPrimesModif(long lowerLimit, long upperLimit, long& result) {
	for (long i = lowerLimit; i < upperLimit; ++i) {
		if (i <= 1) continue;
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

//count of primes using sequential implementation (0 and 1 done correct)
long sequentialSolutionModif(long size) {
	long noPrimes = 0;
	countPrimesModif(0, size, noPrimes);
	return noPrimes;
}

//because of the 0 and 1
void countPrimesModif2(long lowerLimit, long upperLimit, long& result) {
	lowerLimit = (lowerLimit < 2) ? 2 : lowerLimit;
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

//count of primes using sequential implementation (0 and 1 done correct)
long sequentialSolutionModif2(long size) {
	long noPrimes = 0;
	countPrimesModif2(0, size, noPrimes);
	return noPrimes;
}

//with the sqrt
void countPrimesOptimized(long lowerLimit, long upperLimit, long& result) {
	lowerLimit = (lowerLimit < 2) ? 2 : lowerLimit;
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j*j <= i; j++) {
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

long sequentialSolutionOptimized(long size) {
	long noPrimes = 0;
	countPrimesOptimized(0, size, noPrimes);
	return noPrimes;
}


//parallel I (race condition)
long parallelSolution(long size) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread>threads;
	long interval = size / noThreads;
	for (int i = 0; i < noThreads; i++) {
		long lower = i * interval;
		long upper = (i + 1) * interval;
		threads.push_back(thread(countPrimesOptimized, lower, upper, ref(noPrimes)));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	return noPrimes;
}

void countPrimesMutex(long lowerLimit, long upperLimit, long& result, mutex& mutex) {
	//double startTime = omp_get_wtime();
	lowerLimit = (lowerLimit < 2) ? 2 : lowerLimit;
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j * j <= i; j++) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		mutex.lock();
		if (isPrime) {
			result += 1;
		}
		mutex.unlock();
	}
	/*double stopTime = omp_get_wtime();
	printf("\nThread takes %f seconds", stopTime - startTime);*/
}


//parallel II (no race condition solved by using mutex)
long parallelSolutionMutex(long size) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread>threads;
	long interval = size / noThreads;
	mutex lockObject;
	for (int i = 0; i < noThreads; i++) {
		long lower = i * interval;
		long upper = (i + 1) * interval;
		threads.push_back(thread(countPrimesMutex, lower, upper, ref(noPrimes), ref(lockObject)));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}

	return noPrimes;
}

void countPrimesMutexStep(long lowerLimit, long upperLimit, long step, long& result, mutex& mutex) {
	double startTime = omp_get_wtime();

	for (long i = lowerLimit; i < upperLimit; i+=step) {
		if (i <= 1) continue;
		bool isPrime = true;
		for (long j = 2; j * j <= i; j++) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		mutex.lock();
		if (isPrime) {
			result += 1;
		}
		mutex.unlock();
	}
	double stopTime = omp_get_wtime();
	//printf("\nThread takes %f seconds", stopTime - startTime);
}

//parallel III (load balancing + mutex)
long parallelSolutionLoadBalanceWithMutex(long size) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread>threads;
	mutex lockObject;
	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(countPrimesMutexStep, i, size, noThreads, ref(noPrimes), ref(lockObject)));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}

	return noPrimes;
}

//parallel IV (better load balancing + mutex)
long parallelSolutionBetterLoadBalanceWithMutex(long size) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread>threads;
	long start = 1;
	mutex lockObject;
	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(countPrimesMutexStep, start, size, 2*noThreads, ref(noPrimes), ref(lockObject)));
		start += 2;
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	noPrimes += 1;//just one more also for the value 2

	return noPrimes;
}

void countPrimesStep(long lowerLimit, long upperLimit, long step, long& result) {
	double startTime = omp_get_wtime();

	for (long i = lowerLimit; i < upperLimit; i += step) {
		if (i <= 1) continue;
		bool isPrime = true;
		for (long j = 2; j * j <= i; j++) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}
	double stopTime = omp_get_wtime();
	//printf("\nThread takes %f seconds", stopTime - startTime);
}

//parallel V (better load balancing + NO mutex)
long parallelSolutionBetterLoadBalanceWithNoMutex(long size) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread>threads;
	vector<long>results(noThreads, 0);
	long start = 1;
	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(countPrimesStep, start, size, 2 * noThreads, ref(results[i])));
		start += 2;
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	for (int i = 0; i < noThreads; i++) {
		noPrimes += results[i];
	}
	noPrimes += 1;//just one more also for the value 2

	return noPrimes;
}

//best version using sequential implementation
//transforming it in parallel programming
//using open mp
void ompCountPrimes(long lowerLimit, long upperLimit, long& result) {
	lowerLimit = (lowerLimit < 2) ? 2 : lowerLimit;

	long localVariable = 0;
#pragma omp parallel for reduction(+:localVariable)
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j * j <= i; j++) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			localVariable += 1;
		}
	}
	result = localVariable;
}

long ompParallelSolution(long size) {
	long noPrimes = 0;
	ompCountPrimes(0, size, noPrimes);
	return noPrimes;
}

int counterBenchmark = 0;

void benchmank(string description, long size, long (*functionPointer)(long)) {
	counterBenchmark++;
	printf("\n\n----------------------%d------------------", counterBenchmark);
	printf("\nTest for %s", description.c_str());
	double startTime = omp_get_wtime();

	long result = functionPointer(size);

	double stopTime = omp_get_wtime();

	printf("\nResult is %d", result);
	printf("\nTime %f seconds", stopTime - startTime);
}




