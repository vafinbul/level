#include <stdio.h>
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    int size, rank;
    const int a = 5;
    const int b = 3;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int aCols = 7;
    int aRows = 7;
    int bCols = 7;
    int bRows = 7;
    if (aCols != bRows)
    {
        MPI_Finalize();
        return -1;
    }
    if (rank == 0)
    {

        int** a = new int* [aRows];
        for (int i = 0; i < aRows; i++)
        {
            a[i] = new int[aCols];
        }
        int** b = new int* [bRows];
        for (int i = 0; i < bRows; i++)
        {
            b[i] = new int[bCols];
        }
        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < aCols; j++)
            {
                a[i][j] = (i + 1) * (j + 1);
            }
        }
        for (int i = 0; i < bRows; i++)
        {
            for (int j = 0; j < bCols; j++)
            {
                b[i][j] = (i + 1) * (j + 1);
            }
        }
        int* a_ = new int[aRows * aCols];
        int* b_ = new int[bRows * bCols];
        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < aCols; j++)
            {
                a_[i * aCols + j] = a[i][j];
            }
        }
        for (int i = 0; i < bRows; i++)
        {
            for (int j = 0; j < bCols; j++)
            {
                b_[i * bCols + j] = b[i][j];
            }
        }
        for (int i = 1; i < size; i++)
        {
            MPI_Send(a_, aRows * aCols, MPI_INT, i, 10, MPI_COMM_WORLD);
            MPI_Send(b_, bRows * bCols, MPI_INT, i, 20, MPI_COMM_WORLD);
        }

        int** ans = new int* [aRows];
        for (int i = 0; i < aRows; i++)
        {
            ans[i] = new int[bCols];
        }
        for (int i = 1; i < size; i++)
        {
            MPI_Status status;
            int block = aRows / (size - 1) + 1;
            int start = (i - 1) * block;
            int end = i * block;
            if (end > aRows)
            {
                end = aRows;
            }
            if (start > aRows)
            {
                start = aRows;
            }
            if (start == end)
            {
                continue;
            }
            int* ans_ = new int[(end - start) * bCols];
            MPI_Recv(ans_, (end - start) * bCols, MPI_INT, i, 30, MPI_COMM_WORLD, &status);
            for (int j = start; j < end; j++)
            {
                for (int k = 0; k < bCols; k++)
                {
                    ans[j][k] = ans_[(j - start) * bCols + k];
                }
            }
        }
        for (int i = 0; i < aRows; i++)
        {
            for (int j = 0; j < bCols; j++)
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
        int** b = new int* [bRows];
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* buf = new int[count];
        MPI_Recv(buf, count, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        aCols = count / aRows;
        for (int i = 0; i < aRows; i++)
        {
            a[i] = new int[aCols];
            for (int j = 0; j < aCols; j++)
            {
                a[i][j] = buf[i * aCols + j];
            }
        }
        MPI_Probe(0, 20, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* buf2 = new int[count];
        MPI_Recv(buf2, count, MPI_INT, 0, 20, MPI_COMM_WORLD, &status);
        bCols = count / bRows;
        for (int i = 0; i < bRows; i++)
        {
            b[i] = new int[bCols];
            for (int j = 0; j < bCols; j++)
            {
                b[i][j] = buf2[i * bCols + j];
            }
        }
        int block = aRows / (size - 1) + 1;
        int start = (rank - 1) * block;
        int end = rank * block;
        if (end > aRows)
        {
            end = aRows;
        }
        if (start > aRows)
        {
            start = aRows;
        }

        if (start == end)
        {
            MPI_Finalize();
            return 0;
        }

        int** ans = new int* [end - start];
        for (int i = 0; i < end - start; i++)
        {
            ans[i] = new int[bCols];
        }
        for (int i = start; i < end; i++)
        {
            for (int j = 0; j < bCols; j++)
            {
                int sum = 0;
                for (int k = 0; k < aCols; k++)
                {
                    sum += a[i][k] * b[k][j];
                }
                ans[i - start][j] = sum;
            }
        }
        int* ans_ = new int[(end - start) * bCols];
        for (int i = 0; i < end - start; i++)
        {
            for (int j = 0; j < bCols; j++)
            {
                ans_[i * bCols + j] = ans[i][j];
            }
        }
        MPI_Send(ans_, (end - start) * bCols, MPI_INT, 0, 30, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}