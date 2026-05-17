#include<iostream>
#include<omp.h>
#include<vector>
#include<thread>
using namespace std;

int counter = 0;
void printLine() {
	counter++;
	cout << "\n-------------------------------------------Example " << counter << "-------------------------------------------\n";
}

int main() {
	printLine();

#pragma omp parallel
	{
		printf("\nHello from thread %d", omp_get_thread_num());
	}

	printLine();
#pragma omp parallel
	{
		cout << "\nHello from thread " << omp_get_thread_num();
	}

	printLine();

	double startTime = omp_get_wtime();
	for (int i = 0; i <= 100; i++) {
		printf("%d ", i);
	}
	double stopTime = omp_get_wtime();
	cout << "\nTime: " << stopTime - startTime;

	printLine();
	startTime = omp_get_wtime();

#pragma omp parallel for
	for (int i = 0; i <= 100; i++) {
		printf("%d ", i);
	}
	stopTime = omp_get_wtime();
	cout << "\nTime: " << stopTime - startTime;


	printLine();
	//bad solution (it has race condition upon the sum)
	int sum = 0;
#pragma omp parallel for
	for (int i = 0; i < 1000; i++) {
		sum += i;
	}
	cout << "\nTotal sum: " << sum;

	printLine();
	//good solution (it no longer has race condition upon the sum)
	sum = 0;

#pragma omp parallel for reduction(+:sum) schedule(dynamic)
	for (int i = 0; i < 1000; i++) {
		sum += i;
	}
	cout << "\nTotal sum with reduction: " << sum;

	printLine();
	sum = 0;
#pragma omp parallel for
	for (int i = 0; i < 1000; i++) {
#pragma omp critical
		{
			sum += i;
		}
	}
	cout << "\nTotal sum with critical zone: " << sum;

	printLine();
	int x = 1000000;
#pragma omp parallel for
	for (int i = 0; i < 10000000; i++) {
#pragma omp critical
		{
			if (x > 500) {
				x -= 500;
			}
		}
	}

	printLine();
#pragma omp atomic
	counter++;

	return 0;
}