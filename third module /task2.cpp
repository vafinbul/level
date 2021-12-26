#include <stdio.h>
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int N = 101;
	int* x;
	int* y;
	int sum = 0;


	int block = N / size + 1;
	int start = rank * block;
	int end = (rank + 1) * block;
	if (start > N)
	{
		start = N;
	}
	if (end > N)
	{
		end = N;
	}
	int sendcount = end - start;
	int* xdata = new int[block];
	int* ydata = new int[block];
	if (rank == 0)
	{
		x = new int[N];
		y = new int[N];
		for (int i = 0; i < N; i++)
		{
			x[i] = i;
			y[i] = i;
		}
	}
	else
	{
		x = NULL;
		y = NULL;
	}
	MPI_Scatter(x, sendcount, MPI_INT, xdata, sendcount, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(y, sendcount, MPI_INT, ydata, sendcount, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < sendcount; i++)
	{
		sum += xdata[i] * ydata[i];
	}
	int global_sum;
	MPI_Reduce(&sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Scalar product is - %d", global_sum);
	}
	MPI_Finalize();
	return 0;
}