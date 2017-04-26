#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>

#include <mpi.h>

using namespace std;

int main(int argc, char *argv[])
{
    /*<trace_folder> <rand_send>*/
    if (argc != 3)
    {
        cerr << ">Unexpected quantity of arguments, check your comand string." << endl;
        return -1;
    }

    string tr_fold(argv[1]);

    MPI_Init(&argc, &argv);

    int nProc = 0, myRank = 0;
    MPI_Status status;
    MPI_Request req;
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    string file_name = tr_fold +  "/proc_" + to_string(myRank);
    ofstream proc_file(file_name.c_str());

    ifstream rand_send(argv[2]);

    proc_file << myRank << endl;

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

    /*MPI_Bcast(rand_rank_send, nProc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(rand_rank_recv, nProc, MPI_INT, 0, MPI_COMM_WORLD);*/

    for (int i = 0; i < size; i++)
    {
        mas1[i] = i;
        mas2[i] = i;
    }

    if (myRank < nProc / 2)
    {   
        proc_file << nProc - 1 - myRank << " " << size << endl;
        MPI_Send(mas1, size, MPI_DOUBLE, nProc - 1 - myRank, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(mas3, size, MPI_DOUBLE, abs(nProc - 1 - myRank), 0, MPI_COMM_WORLD, &status);
    }

    proc_file << rand_rank_send[myRank] << " " << size << endl;
    MPI_Isend(mas2, size, MPI_DOUBLE, rand_rank_send[myRank], 0, MPI_COMM_WORLD, &req);
    //cout << myRank << endl;
    MPI_Irecv(mas4, size, MPI_DOUBLE, rand_rank_recv[myRank], 0, MPI_COMM_WORLD, &req);


    delete [] mas1;
    delete [] mas2;
    delete [] mas3;
    delete [] mas4;

    delete [] rand_rank_send;
    delete [] rand_rank_recv;

    proc_file.close();
    MPI_Finalize();
    return 0;
}