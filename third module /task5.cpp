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
    int** b;
    int* a_;
    int* b_;
    int globalMax = NULL;
    int** c;
    int* c_;
    if (rank == 0)
    {
        a = new int* [m];
        b = new int* [m];
        c = new int* [m];
        a_ = new int[m * n];
        b_ = new int[m * n];
        c_ = new int[m * n];
        for (int i = 0; i < m; i++)
        {
            a[i] = new int[n];
            b[i] = new int[n];
            c[i] = new int[n];
            for (int j = 0; j < n; j++)
            {
                a[i][j] = (i + 1) * (j + 1);
                a_[i * n + j] = a[i][j];
                b[i][j] = (i + 1) * (j + 1);
                b_[i * n + j] = b[i][j];
            }
        }
    }
    else
    {
        a = NULL;
        b = NULL;
        c = NULL;
        a_ = NULL;
        b_ = NULL;
        c_ = NULL;
    }
    int block = m * n / size + 1;
    int start = block * rank;
    int end = block * (rank + 1);
    if (start > m * n)
    {
        start = m * n;
    }
    if (end > m * n)
    {
        end = m * n;
    }
    int* adata = new int[block];
    int* bdata = new int[block];
    int count = end - start;
    MPI_Scatter(a_, block, MPI_INT, adata, block, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b_, block, MPI_INT, bdata, block, MPI_INT, 0, MPI_COMM_WORLD);
    int* cdata = new int[count];
    for (int i = 0; i < count; i++)
    {
        cdata[i] = adata[i] * bdata[i];
    }
    MPI_Gather(cdata, count, MPI_INT, c_, count, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                c[i][j] = c_[i * n + j];
                printf("%10d ", c[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}