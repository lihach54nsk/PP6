#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <chrono>

using namespace std;

int input[1000000];
int output[sizeof(input) / sizeof(int)];

bool Prime(long long n)
{
	for (int i = 2; i <= sqrt(n); i++)
		if (n%i == 0)
			return false;
	return true;
}

void Calc()
{
	int j = 0;
	int size = sizeof(input) / sizeof(int);
	for (int i = 0; i < size; i++)
	{
		if (Prime(input[i]) == true) { output[j] = input[i]; j++; }
		else continue;
	}
}

void Quest()
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		for (int c = 1; c < 999999; c++)
		{
			input[c - 1] = c;
			//cout << input[c - 1] << endl;
		}
	}

	MPI_Scatter(&input, 1, MPI_INT, &output, 1, MPI_INT, 0, MPI_COMM_WORLD);

	auto begin = chrono::high_resolution_clock::now();
	Calc();
	auto end = chrono::high_resolution_clock::now();

	//MPI_Reduce(&output, &check, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		int i = 0;
		while (output[i] != 0)
		{
			cout << "sum: " << output[i] << endl;
			i++;
		}
		cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
	}
}

int main(int argc, char *argv[])
{
	/*for (int c = 1; c < 999999; c++)
	{
		input[c - 1] = c;
	}

	auto begin = chrono::high_resolution_clock::now();
	Calc();
	auto end = chrono::high_resolution_clock::now();

	cout << "Time without MPI: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;*/

	MPI_Init(&argc, &argv);	

	Quest();

	MPI_Finalize();

	return 0;
}