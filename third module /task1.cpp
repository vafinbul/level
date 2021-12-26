#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    float N = 101;
    int* x;
    int sum = 0;

    int block = ceil(N / size);

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
    int recvcount = end - start;
    int* data = new int[recvcount];

    if (rank == 0)
    {
        x = new int[N];
        for (int i = 0; i < N; i++)
        {
            x[i] = i * pow(-1, i % 2);
        }
    }
    else x = NULL;

    MPI_Scatter(x, block, MPI_INT, data, recvcount, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < recvcount; i++)
    {
        sum += abs(data[i]);
    }
    int global_sum;
    MPI_Reduce(&sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Sum = %d\n", global_sum);
    }
    MPI_Finalize();
    return 0;
}