#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <chrono>
#include <random>
#include <ctime>

using namespace std;

void Quest()
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Comm first, second;

	srand(time(0));
	auto color = 1 + rand() % 2;

	MPI_Comm_split(MPI_COMM_WORLD, 1, 1, &first);
	MPI_Comm_split(MPI_COMM_WORLD, 2, 1, &second);

	int newRank, newRank2, newSize, newSize2;
	MPI_Comm_rank(first, &newRank);
	MPI_Comm_size(first, &newSize);
	MPI_Comm_rank(second, &newRank2);
	MPI_Comm_size(second, &newSize2);

	int input, input2, output, output2 = 100000;
	int result = 0; int result2 = 100000;

	if (rank == 0)
	{
		for (int i = 1; i < 4; i++)
		{
			input = rand();
			MPI_Send(&input, 1, MPI_INT, i, 123, first);
			cout << "input: " << input << endl;
		}

		for (int j = 1; j < 4; j++)
		{
			input2 = rand();
			MPI_Send(&input2, 1, MPI_INT, j, 1234, second);
			cout << "input2: " << input2 << endl;
		}
	}
	else
	{
		MPI_Status status, status2;

		MPI_Recv(&output, 1, MPI_INT, 0, 123, first, &status);
		MPI_Recv(&output2, 1, MPI_INT, 0, 1234, second, &status2);
	}

	MPI_Reduce(&output, &result, 1, MPI_INT, MPI_MAX, 0, first);
	MPI_Reduce(&output2, &result2, 1, MPI_INT, MPI_MIN, 0, second);

	if (newRank == 0)
		cout << "Result: " << result << endl;
	if (newRank2 == 0)
		cout << "Result2: " << result2 << endl;
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);	

	Quest();

	MPI_Finalize();

	return 0;
}