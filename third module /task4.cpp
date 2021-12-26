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

    const int m = 10;
    const int n = 16;
    int** a;
    int* a_;
    int globalMax = NULL;
    if (rank == 0)
    {
        a = new int* [m];
        a_ = new int[m * n];
        for (int i = 0; i < m; i++)
        {
            a[i] = new int[n];
            for (int j = 0; j < n; j++)
            {
                a[i][j] = (i + 1) * (j + 1);
                a_[j * m + i] = a[i][j];
            }
        }
    }
    else
    {
        a = NULL;
        a_ = NULL;
    }
    int block = (n / size + 1) * m;
    int start = rank * block;
    int end = (rank + 1) * block;
    if (start > n * m)
    {
        start = n * m;
    }
    if (end > n * m)
    {
        end = n * m;
    }
    int count = end - start;
    int* data = new int[block];
    MPI_Scatter(a_, block, MPI_INT, data, block, MPI_INT, 0, MPI_COMM_WORLD);
    int cols = count / m;
    if (count != 0)
    {
        int max = NULL;
        int** arr = new int* [cols];
        for (int i = 0; i < cols; i++)
        {
            arr[i] = new int[m];
            for (int j = 0; j < m; j++)
            {
                arr[i][j] = data[i * m + j];
            }
        }
        for (int i = 0; i < cols; i++)
        {
            int sum = 0;
            for (int j = 0; j < m; j++)
            {
                sum += arr[i][j];
            }
            if (max != NULL)
            {
                if (max < sum)
                {
                    max = sum;
                }
            }
            else
            {
                max = sum;
            }
        }
        MPI_Reduce(&max, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    }
    if (rank == 0)
    {
        printf("A norm is - %d", globalMax);
    }
    MPI_Finalize();
    return 0;
}