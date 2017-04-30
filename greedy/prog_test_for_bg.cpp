#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>

#include <mpi.h>
#include <ctime>

using namespace std;

int main(int argc, char *argv[])
{
    /*<trace_folder> <rand_send>*/
    if (argc != 2)
    {
        cerr << ">Unexpected quantity of arguments, check your comand string." << endl;
        return -1;
    }

    double time = clock();

    MPI_Init(&argc, &argv);

    int nProc = 0, myRank = 0;
    MPI_Status status;
    MPI_Request req;
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    ifstream rand_send(argv[2]);

    double *mas1 = NULL, *mas2 = NULL, *mas3 = NULL, *mas4 = NULL;    
    
    int size = 1000;
    mas1 = new double [size];
    mas2 = new double [size];
    mas3 = new double [size];
    mas4 = new double [size];

    int *rand_rank_send = new int [nProc];
    int *rand_rank_recv = new int [nProc];

    for (int i = 0; i < nProc; i++)
    {
        rand_send >> rand_rank_send[i];
        rand_send >> rand_rank_recv[i];
    }

    for (int i = 0; i < size; i++)
    {
        mas1[i] = i;
        mas2[i] = i;
    }

    if (myRank < nProc / 2)
    {   
        MPI_Send(mas1, size, MPI_DOUBLE, nProc - 1 - myRank, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(mas3, size, MPI_DOUBLE, abs(nProc - 1 - myRank), 0, MPI_COMM_WORLD, &status);
    }

    MPI_Isend(mas2, size, MPI_DOUBLE, rand_rank_send[myRank], 0, MPI_COMM_WORLD, &req);
    MPI_Irecv(mas4, size, MPI_DOUBLE, rand_rank_recv[myRank], 0, MPI_COMM_WORLD, &req);


    delete [] mas1;
    delete [] mas2;
    delete [] mas3;
    delete [] mas4;

    delete [] rand_rank_send;
    delete [] rand_rank_recv;

    double fin_time = clock() - time;

    MPI_Reduce(&fin_time, &time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (myRank == 0)
        cout << ">Time of computation = " << time / CLOCKS_PER_SEC << endl;

    rand_send.close();
    MPI_Finalize();
    return 0;
}
