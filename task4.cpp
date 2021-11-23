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

    if (rank == 0)
    {
        int* arr = new int[size];
        cout << "OK1" << endl;
        for (int i = 0; i < 10; i++) arr[i] = i;
        cout << "OK2" << endl;

        for (int i = 1; i < size; i++) MPI_Send(arr, 10, MPI_INT, i, 5, MPI_COMM_WORLD);
        cout << "OK1" << endl;

    }
    else
    {
        int count;
        MPI_Status status;

        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int* buf = new int[count];

        MPI_Recv(buf, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        cout << "Process:" << rank << " || Count: " << count << " || Array: ";
        for (int i = 0; i < count; i++)
        {
            cout << buf[i] << " ";
        }
        cout << "\n";
    }
    MPI_Finalize();
    return 0;
}

