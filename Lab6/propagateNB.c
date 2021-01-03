#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 
 
int main(int argc, char **argv) 
{ 
    int rank,ranksent,size,source,dest,tag,i,len;
    char name[20];
    len=20;
    MPI_Status status; 
    MPI_Request request; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    MPI_Get_processor_name(name,&len);

    int* number = (int*) malloc(sizeof(int));
    *number = 1;

    while(*number >= 0){
        if(rank == 0){
            printf("Enter a number\n");
            scanf("%d", number);
            printf("Process 0 got a %d\n", *number); 
        } else {
            int prev = rank - 1;
            MPI_Irecv(number,1,MPI_INT,prev,MPI_ANY_TAG,MPI_COMM_WORLD,&request); 
            MPI_Wait(&request, &status);
            printf("Process %d got %d from process %d\n",rank, *number, status.MPI_SOURCE); 
        }

        int next = rank + 1;
        if(next == size)
            next = 0;
        tag = 0;
        MPI_Isend(number,1,MPI_INT,next,tag,MPI_COMM_WORLD,&request);

        if(rank == 0){
            int prev = rank - 1;
            MPI_Irecv(number,1,MPI_INT,prev,MPI_ANY_TAG,MPI_COMM_WORLD,&request); 
            MPI_Wait(&request, &status);
            printf("Process %d got %d from process %d\n",rank, *number, status.MPI_SOURCE); 
        }
    }

    MPI_Finalize(); 
    return(0); 
}