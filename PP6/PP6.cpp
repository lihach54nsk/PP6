#include "pch.h"
#include <iostream>
#include <mpi.h>

using namespace std;

double Integration(double from, double to, int splitCount, double(*calcFunc)(double x))
{
	auto step = (to - from) / (double)splitCount;
	auto sum = 0.0;

	for (int i = 0; i < splitCount; i++)
	{
		auto x1 = from + i * step;
		auto x2 = from + (i + 1) * step;
		sum += calcFunc((x1 + x2) / 2.0) * (x2 - x1);
	}

	return sum;
}

int main(int argc, char *argv[])
{
	double from, to;
	int splitCount;
	from = 0; to = 1000; splitCount = 100000;
	MPI_Init(&argc, &argv);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	double result = Integration(from, to, splitCount, [](double x) { return cos(x); });
	cout << "Result = " << result << " in rank " << rank << " of " << size << endl;
	MPI_Finalize();

	return 0;
}