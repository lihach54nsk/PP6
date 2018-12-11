#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <chrono>
#include <random>
#include <ctime>
#include <string>

using namespace std;

void Quest()
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Comm first;

	srand(rank);
	int color = rank % 2;

	MPI_Comm_split(MPI_COMM_WORLD, rank % 2, 1, &first);

	int newRank, newSize;
	MPI_Comm_rank(first, &newRank);
	MPI_Comm_size(first, &newSize);

	int input = 0;
	int result = 0;

	input = rand();
	cout << "Color: " << color << " value: "<< input << endl;


	if (color == 0)
	{
		MPI_Reduce(&input, &result, 1, MPI_INT, MPI_MAX, 0, first);
	}
	else
	{
		MPI_Reduce(&input, &result, 1, MPI_INT, MPI_MIN, 0, first);
	}

	if (newRank == 0)
	{
		string str = color == 0 ? "Max: " : "Min: ";
		cout << str << result << endl;
	}
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);	

	Quest();

	MPI_Finalize();

	return 0;
}