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

void Quest(IntegrInput input[], IntegrInput output[])
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Datatype InputParams_MPI;

	int length[3] = { 1, 1, 1 };

	MPI_Aint offsets[3] =
	{
		offsetof(IntegrInput, from),
		offsetof(IntegrInput, to),
		offsetof(IntegrInput, splitCount)
	};

	MPI_Datatype types[3] = { MPI_DOUBLE, MPI_DOUBLE, MPI_INT };

	MPI_Type_create_struct(3, length, offsets, types, &InputParams_MPI);
	MPI_Type_commit(&InputParams_MPI);

	int sendCount[4] = { 1, 1, 1, 1 }, disCount[4] = { 1, 1, 1, 1 };

	if (rank == 0)
	{
		cout << "In!" << endl;
		MPI_Scatterv(&input, sendCount, disCount, InputParams_MPI, &output, 1, InputParams_MPI, 0, MPI_COMM_WORLD);
		/*MPI_Scatterv(&input->from, sendCount, disCount, MPI_DOUBLE, &output->from, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Scatterv(&input->to, sendCount, disCount, MPI_DOUBLE, &output->to, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Scatterv(&input->splitCount, sendCount, disCount, MPI_DOUBLE, &output->splitCount, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);*/
		cout << "Sent!" << endl;
	}
	else
	{
		cout << "Here!";
		MPI_Gatherv(&input, sendCount[rank], InputParams_MPI, &output, sendCount, disCount, InputParams_MPI, 0, MPI_COMM_WORLD);
		/*MPI_Gatherv(&input->from, sendCount[rank], MPI_DOUBLE, &output->from, sendCount, disCount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Gatherv(&input->to, sendCount[rank], MPI_DOUBLE, &output->to, sendCount, disCount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Gatherv(&input->splitCount, sendCount[rank], MPI_DOUBLE, &output->splitCount, sendCount, disCount, MPI_DOUBLE, 0, MPI_COMM_WORLD);*/
		cout << "Here!";

		for (int i = 0; i < size; i++)
		{
			cout << output[i].from << output[i].to << output[i].splitCount << endl;
			double result = Integration(*output, [](double x) { return cos(x); });
			cout << "Result = " << result << " in rang " << rank << " of " << size << endl;
		}		
	}

	MPI_Finalize();
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	IntegrInput custom;

	custom.from = 0; custom.to = 1000; custom.splitCount = 100000;

	int size = 4;

	IntegrInput input[4], output[4];

	auto stepByProcess = (custom.to - custom.from) / size;

	for (int i = 1; i <= size; i++)
	{
		input[i - 1].from = custom.from + i * stepByProcess;
		input[i - 1].to = custom.to + i * stepByProcess;
		input[i - 1].splitCount = custom.splitCount;
		cout << input[i - 1].from << endl << input[i - 1].to << endl << input[i - 1].splitCount << endl;
	}

	Quest(input, output);

	return 0;
}