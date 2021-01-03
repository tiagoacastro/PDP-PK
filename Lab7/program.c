#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 
 
int main(int argc, char **argv) 
{ 
    int rank, num_errors = 0, tot_errors = 0, len = 20;
    char name[20];
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    MPI_Get_processor_name(name,&len);

    int* number = (int*) malloc(sizeof(int));
    *number = 1;

    while(*number >= 0){
        if(rank == 0){
            printf("Enter a number\n");
            scanf("%d", number);
        }

        MPI_Bcast(number,1,MPI_INT,0,MPI_COMM_WORLD);
        printf("P[%d] after bcast: %d   ---    %s\n",rank, *number, name); 
        
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize(); 
    return(0); 
}