#include "pch.h"
#include <iostream>
#include <mpi.h>

using namespace std;

bool Prime(long long n)
{
	for (int i = 2; i <= sqrt(n); i++)
		if (n%i == 0)
			return false;
	return true;
}

void Quest()
{

	//Full(input);
	/*for (int i = 0; i < 4; i++)
	{
		cout << "Here we go:" << input[i].from << " " << input[i].to << " " << input[i].splitCount << endl;
	}*/

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

	MPI_Scatter(&input, 1, InputParams_MPI, &output, 1, InputParams_MPI, 0, MPI_COMM_WORLD);

	double sum = 0;
	double check = 0;

	MPI_Reduce(&sum, &check, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		cout << "sum: " << check;
	}
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);	

	Quest();

	MPI_Finalize();

	return 0;
}