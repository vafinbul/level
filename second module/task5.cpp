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

    int aCols = 5;
    int aRows = 7;
    if (rank == 0)
    {

        int** a = new int* [aRows];
        for (int i = 0; i < aRows; i++)
        {
            a[i] = new int[aCols];
        }
        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < aCols; j++)
            {
                a[i][j] = aCols * i + (j + 1);
            }
        }
        int* a_ = new int[aRows * aCols];
        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < aCols; j++)
            {
                a_[i * aCols + j] = a[i][j];
            }
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Send(a_, aRows * aCols, MPI_INT, i, 10, MPI_COMM_WORLD);
        }
        int** ans = new int* [aCols];
        for (int i = 0; i < aCols; i++)
        {
            ans[i] = new int[aRows];
        }
        for (int i = 1; i < size; i++)
        {
            MPI_Status status;
            int block = aCols / (size - 1) + 1;
            int start = (i - 1) * block;
            int end = i * block;
            if (end > aCols)
            {
                end = aCols;
            }
            if (start > aCols)
            {
                start = aCols;
            }
            if (start == end)
            {
                continue;
            }
            int* ans_ = new int[(end - start) * aRows];

            MPI_Recv(ans_, (end - start) * aRows, MPI_INT, i, 30, MPI_COMM_WORLD, &status);
            for (int i = start; i < end; i++)
            {
                for (int j = 0; j < aRows; j++)
                {
                    ans[i][j] = ans_[(i - start) * aRows + j];
                }
            }


        }
        for (int i = 0; i < aCols; i++)
        {
            for (int j = 0; j < aRows; j++)
            {
                printf("%10d ", ans[i][j]);
            }
            printf("\n");
        }
    }
    else
    {

        int count;
        MPI_Status status;
        int** a = new int* [aRows];
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* buf = new int[count];
        MPI_Recv(buf, count, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < aRows; i++)
        {
            a[i] = new int[aCols];
            for (int j = 0; j < aCols; j++)
            {
                a[i][j] = buf[i * aCols + j];
            }
        }

        int block = aCols / (size - 1) + 1;
        int start = (rank - 1) * block;
        int end = rank * block;
        if (end > aCols)
        {
            end = aCols;
        }
        if (start > aCols)
        {
            start = aCols;
        }

        if (start == end)
        {
            MPI_Finalize();
            return 0;
        }

        int** ans = new int* [end - start];
        for (int i = 0; i < end - start; i++)
        {
            ans[i] = new int[aRows];
        }

        for (int i = start; i < end; i++)
        {
            for (int j = 0; j < aRows; j++)
            {
                ans[i - start][j] = a[j][i];
            }
        }

        int* ans_ = new int[(end - start) * aRows];
        for (int i = start; i < end; i++)
        {
            for (int j = 0; j < aRows; j++)
            {
                ans_[(i - start) * aRows + j] = ans[i - start][j];
            }
        }
        MPI_Send(ans_, (end - start) * aRows, MPI_INT, 0, 30, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}