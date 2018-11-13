#include "pch.h"
#include <iostream>
#include <mpi.h>

using namespace std;

struct IntegrInput
{
	double from;
	double to;
	double step;
	double(*fromFunc)(double x);
};

void* Integraing(void *param)
{
	auto input = static_cast<IntegrInput*>(param);
	double* sum = new double();

	for (auto x1 = input->from; x1 < input->to; x1 += input->step)
	{
		auto x2 = x1 + input->step;
		*sum += input->fromFunc((x1 + x2) / 2.0) * (x2 - x1);
	}

	delete input;
	return static_cast<void*>(sum);
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	cout << "Hello World!\n" << rank << "of" << size << endl;
	MPI_Finalize();

	return 0;
}