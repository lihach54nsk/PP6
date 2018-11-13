#include "pch.h"
#include <iostream>
#include <mpi.h>

using namespace std;

struct IntegrInput
{
	double from;
	double to;
	double splitCount;
};

double Integration(IntegrInput input, double(*calcFunc)(double x))
{
	auto step = (input.to - input.from) / input.splitCount;
	auto sum = 0.0;

	for (int i = 0; i < input.splitCount; i++)
	{
		auto x1 = input.from + i * step;
		auto x2 = input.from + (i + 1) * step;
		sum += calcFunc((x1 + x2) / 2.0) * (x2 - x1);
	}

	return sum;
}

int main(int argc, char *argv[])
{
	IntegrInput input, output;
	input.from = 0; input.to = 1000; input.splitCount = 100000;

	MPI_Init(&argc, &argv);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		MPI_Bcast(&input.from, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(&input.to, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(&input.splitCount, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}
	else
	{
		/*for (int i = 0; i < size; i++) 
		{*/
			MPI_Gather(&output.from, 1, MPI_DOUBLE, &output.from, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			MPI_Gather(&output.to, 1, MPI_DOUBLE, &output.to, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			MPI_Gather(&output.splitCount, 1, MPI_DOUBLE, &output.splitCount, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			double result = Integration(output, [](double x) { return cos(x); });
			cout << "Result = " << result << " in rang " << rank << " of " << size << endl;
		/*}*/

		MPI_Finalize();
	}	

	return 0;
}