#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <chrono>

using namespace std;

int Prime(long long from, long long n)
{
	for (long long i = from; i <= sqrt(n); i++)
		if (n % i == 0)
			return 0;
	return 1;
}

void Quest()
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	long long inputFrom[4], inputTo[4];

	long long value = 0;

	if (rank == 0)
	{		
		cout << "Write value: "; cin >> value;

		long long from = 2;
		long long to = sqrt(value);
		long long step = (to - from) / size;

		cout << "Step = " << step << " To = " << to << " Size = " << size << endl;

		for (int i = 0; i < size; i++)
		{
			inputFrom[i] = from + step * i;
			inputTo[i] = from + step * i + step;

			cout << "inputFrom = " << inputFrom[i] << " inputTo = " << inputTo[i] << endl;
		}
	}

	long long outputFrom[4], outputTo[4];
	
	MPI_Scatter(&inputFrom, 4, MPI_LONG_LONG, &outputFrom, 4, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
	MPI_Scatter(&inputTo, 4, MPI_LONG_LONG, &outputTo, 4, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < size; i++)
		{
			cout << "outputFrom = " << outputFrom[i] << " outputTo = " << outputTo[i] << endl;
		}
	}

	//cout << "Initialization complete!";
	auto begin = chrono::high_resolution_clock::now();
	int sum = 0;
	sum = sum + Prime(outputFrom[rank], outputTo[rank]);
	//int check = 0;
	//cout << "sum " << sum << endl;
	
	auto end = chrono::high_resolution_clock::now();
	cout << "Work time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;

	if (rank == 0)
	{
		if (sum > 0) cout << "Chislo prostoe" << endl;
		else cout << "Chislo neprostoe" << endl;
	}
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);	

	Quest();

	MPI_Finalize();

	return 0;
}