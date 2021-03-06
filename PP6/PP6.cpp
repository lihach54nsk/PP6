#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <chrono>

using namespace std;

struct IntegrInput
{
	double from;
	double to;
	double splitCount;
};

IntegrInput Full(IntegrInput input[])
{
	IntegrInput custom;

	cout << "Vvedite interval" << endl << "Ot: ";
	cin >> custom.from;
	cout << endl << "Do: ";
	cin >> custom.to;
	cout << endl << "Vvedite kolichestvo razbienij: ";
	cin >> custom.splitCount; cout << endl;

	int size = 4;
	auto stepByProcess = (custom.to - custom.from) / size;

	for (int i = 1; i <= size; i++)
	{
		input[i - 1].from = custom.from + (i - 1) * stepByProcess;
		input[i - 1].to = custom.from + i * stepByProcess;
		input[i - 1].splitCount = custom.splitCount;
	}
	return *input;
}

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

void Quest()
{
	IntegrInput input[4];

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

	MPI_Datatype types[3] = { MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE };

	MPI_Type_create_struct(3, length, offsets, types, &InputParams_MPI);
	MPI_Type_commit(&InputParams_MPI);

	if (rank == 0)
	{
		Full(input);
	}

	IntegrInput output;
	auto begin = chrono::high_resolution_clock::now();
	MPI_Scatter(&input, 1, InputParams_MPI, &output, 1, InputParams_MPI, 0, MPI_COMM_WORLD);

	double sum = 0;
	sum = sum + Integration(output, [](double x) { return cos(x); });
	double check = 0;

	MPI_Reduce(&sum, &check, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	auto end = chrono::high_resolution_clock::now();
	if (rank == 0)
	{
		cout << "sum: " << check << endl << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
	}
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);	

	Quest();

	MPI_Finalize();

	return 0;
}