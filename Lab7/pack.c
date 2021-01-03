#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static struct timespec start,stop;

void tstart(void)
{
	clock_gettime( CLOCK_REALTIME, &start);	
}

double tstop(void)
{
	clock_gettime( CLOCK_REALTIME, &stop);
	return (double)( stop.tv_sec - start.tv_sec ) + (double)( stop.tv_nsec - start.tv_nsec )/1.0e9;
}

int main(int argc, char *argv[])
{
    int rank, size;
    int i;
    char c[20], c1[20], c2[20];
    float f;
    char buffer[100];
    int position = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 2)
    {
        printf("Please run with 2 processes.\n");fflush(stdout);
        MPI_Finalize();
        return 1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        for (i=0; i<20; i++)
            c[i] = i;
        for (i=0; i<20; i++)
            c1[i] = i;
        for (i=0; i<20; i++)
            c2[i] = i;
        i = 20;
        f = 4.2;
        MPI_Pack(c, 20, MPI_CHAR, buffer, 100, &position, MPI_COMM_WORLD);
        MPI_Pack(&i, 1, MPI_INT, buffer, 100, &position, MPI_COMM_WORLD);
        MPI_Pack(&f, 1, MPI_FLOAT, buffer, 100, &position, MPI_COMM_WORLD);
        MPI_Pack(c1, 20, MPI_CHAR, buffer, 100, &position, MPI_COMM_WORLD);
        MPI_Pack(c2, 20, MPI_CHAR, buffer, 100, &position, MPI_COMM_WORLD);
        tstart();
        MPI_Send(buffer, position, MPI_PACKED, 1, 100, MPI_COMM_WORLD);
        float t = tstop();   
   		printf("Execution time = %f\n", t); 
    }

    if (rank == 1)
    {
        MPI_Recv(buffer, 100, MPI_PACKED, 0, 100, MPI_COMM_WORLD, &status);
        MPI_Unpack(buffer, 100, &position, c, 20, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 100, &position, &i, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 100, &position, &f, 1, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 100, &position, c1, 20, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 100, &position, c2, 20, MPI_CHAR, MPI_COMM_WORLD);
        printf("c[0] = %d\n...\nc[19] = %d\ni=%d\nf=%f\nc1[0] = %d\n...\nc1[19] = %d\nc2[0] = %d\n...\nc2[19] = %d\n", 
        (int)c[0], (int)c[19], i, f, (int)c1[0], (int)c1[19], (int)c2[0], (int)c2[19]);fflush(stdout);
    }

    MPI_Finalize();
    return 0;
} 
