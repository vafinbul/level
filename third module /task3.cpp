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
    const int Rows = 7;
    const int Cols = 7;
    int** a;
    int* a_;
    int b[Cols];
    if (rank == 0)
    {
        a = new int* [Rows];
        for (int i = 0; i < Rows; i++)
        {
            a[i] = new int[Cols];
        }
        for (int i = 0; i < Rows; i++)
        {
            for (int j = 0; j < Cols; j++)
            {
                a[i][j] = (i+1)*(j+1);
            }
        }
        for (int j = 0; j < Cols; j++)
        {
            b[j] = j+1;
        }
        a_ = new int[Rows * Cols];
        for (int i = 0; i < Rows; i++)
        {
            for (int j = 0; j < Cols; j++)
            {
                a_[i * Cols + j] = a[i][j];
            }
        }
    }
    else {
        a = NULL;
        a_ = NULL;
    }
    int block = (Rows / size + 1) * Cols;
    int start = rank * block;
    int end = (rank + 1) * block;
    if (start > Rows * Cols)
    {
        start = Rows * Cols;
    }
    if (end > Rows * Cols)
    {
        end = Rows * Cols;
    }
    int count = end - start;
    int* data = new int[block];
    MPI_Scatter(a_, count, MPI_INT, data, block, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, Cols , MPI_INT, 0, MPI_COMM_WORLD);
    int aRows = count / Cols;
    a = new int* [aRows];
    for (int i = 0; i < aRows; i++)
    {
        a[i] = new int[Cols];
    }
    for (int i = 0; i < aRows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            a[i][j] = data[i * Cols + j];
        }
    }
    int* ans_ = new int[aRows];
    for (int i = 0; i < aRows; i++)
    {
        int sum = 0;
        for (int j = 0; j < Cols; j++)
        {
            sum += a[i][j] * b[j];
        }
        ans_[i] = sum;
    }
    int* ans;
    if (rank == 0)
    {
        ans = new int[Rows];
    }
    else
    {
        ans = NULL;
    }
    MPI_Gather(ans_, aRows, MPI_INT, ans, aRows, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        for (int i = 0; i < Rows; i++)
        {
            printf("%d ", ans[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}