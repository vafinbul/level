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

    if (rank == 0)
    {
        int N = 100;
        int* x = new int[100];
        int* y = new int[100];
        for (int i = 0; i < N; i++)
        {
            x[i] = -i;
            y[i] = i;
        }
        cout << "before swapping\n";
        for (int j = 0; j < N; j++)
        {
            cout << "x=" << x[j] << " y=" << y[j] << "\n";
        }
        const int block = N / (size - 1) + (N / (size - 1)) % 2 + 2;
        for (int i = 1; i < size; i++)
        {
            int* arr = new int[block];
            int start = (i - 1) * (block);
            int end = i * (block);
            if (end > N) {
                end = N;
            }
            for (int j = start; j < end; j++)
            {
                arr[j - start] = x[j];
            }
            MPI_Send(arr, end - start, MPI_INT, i, 5, MPI_COMM_WORLD);
            for (int j = start; j < end; j++)
            {
                arr[j - start] = y[j];
            }
            MPI_Send(arr, end - start, MPI_INT, i, 6, MPI_COMM_WORLD);

        }
        for (int i = 1; i < size; i++)
        {
            int count;
            MPI_Status status;

            MPI_Probe(i, 7, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);
            int* buf = new int[count];
            MPI_Recv(buf, count, MPI_INT, i, 7, MPI_COMM_WORLD, &status);
            MPI_Probe(i, 8, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);
            int* buf2 = new int[count];
            MPI_Recv(buf2, count, MPI_INT, i, 8, MPI_COMM_WORLD, &status);
            for (int j = 0; j < count; j++)
            {
                cout << "x=" << buf[j] << " y=" << buf2[j] << "\n";
            }
            cout << "from process " << i << "\n";
        }
    }
    else
    {
        int count;
        MPI_Status status;

        MPI_Probe(0, 5, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* buf = new int[count];
        MPI_Recv(buf, count, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);

        MPI_Probe(0, 6, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* buf2 = new int[count];
        MPI_Recv(buf2, count, MPI_INT, 0, 6, MPI_COMM_WORLD, &status);

        MPI_Send(buf2, count, MPI_INT, 0, 7, MPI_COMM_WORLD);
        MPI_Send(buf, count, MPI_INT, 0, 8, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}