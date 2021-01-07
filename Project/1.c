#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h" 

#define FLOORS 6
#define START_FLOOR 2
#define CAPACITY 5

/*
--------------------------------------------------------------------------------------
Commands to run the program
    • mpicc 1.c -o empi
    • mpirun --hostfile hostfile -np 10 --display-map ./empi

The lift is in a 6 floor building and starts at the second floor and can take up to 5 people. All of this is editable above.
The lift, if it has someone at the floor it is in, will grab the people there as long as they want to go the same way as the
person who first contacted the elevator. In the case of the elevator arriving its destination, it will let any people 
there waiting for it decide the next stop. If there is no one there, it will go to the spot where the first person that 
"presses the button" says. For example, if the elevator is in 2 and is going to 4, but in 3 finds someone going to 5, 
it will drop off the first person at 4 and continue to 5 grabing anyone else in the way that is going the same way. 
--------------------------------------------------------------------------------------
*/
 
int main(int argc, char **argv) 
{ 
    int rank,size,i,j,len,floor,from,to;
    char name[20];
    len=20;
    MPI_Status status; 
    MPI_Request request; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    MPI_Get_processor_name(name,&len);
    
    int* token = (int*) malloc(sizeof(int));
    //if the person is in the elevator
    bool in = false;
    //if the person reach the destination
    bool out = false;
    //keeps the ranks of the processes of the people inside the elevator
    int elev[CAPACITY];
    //number of people inside the elevator
    int people = 0;
    //number of people that arrived at destination (starts at one because of root process)
    int arrived = 1;
    bool print = true;

    floor = -1;
    from = -1;
    to = -1;
    if(rank == 0){
        //'floor' represents the current place where the elevator is
        floor = START_FLOOR;
        *token = floor;
    } else {
        //'from' represents the floor where they are and 'to' represents the floor they want to go
        srand(time(NULL) + rank);
        from = rand() % FLOORS; 
        do {
            to = rand() % FLOORS; 
        } while(from == to);
        printf("P%d - %d to %d\n\n", rank, from, to);
    }

    while(true){
        *token = arrived;
        //sends the number of people that arrived
        MPI_Bcast(token,1,MPI_INT,0,MPI_COMM_WORLD);
        if(*token == size){
            printf("P%d DISCONNECT\n", rank);
            break;
        }

        *token = floor;
        //sends the current location of the elevator
        MPI_Bcast(token,1,MPI_INT,0,MPI_COMM_WORLD);

        if(rank == 0){
            if(print)
                printf("---E - F%d\n", floor);
            else
                print = true;

            //Checks who has arrived
            for(i = 1; i < size; i++){
                MPI_Recv(token,1,MPI_INT,MPI_ANY_SOURCE,3,MPI_COMM_WORLD,&status); 
                if(*token == 1){
                    bool move = false;
                    for(j = 0; j < people-1; j++){
                        if(elev[j] == status.MPI_SOURCE)
                            move = true;
                        if(move)
                            elev[j] = elev[j + 1];
                    }
                    people--;
                    arrived++;
                }
            }

            int newpeople = 0;
            //sees if there are people at that floor to go in
            for(i = 1; i < size; i++){
                MPI_Recv(token,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status); 
                if(*token == 1 && people + newpeople <= CAPACITY){
                    elev[newpeople] = status.MPI_SOURCE;
                    newpeople++;
                }
            }

            //if there are people at that floor to go in
            if(newpeople > 0){
                //if elevator has a destination already or not
                if(to == -1 || to == floor){
                    //if it doesn't, get destination from the first person in and inform it that it can go in
                    MPI_Recv(token,1,MPI_INT,elev[0],1,MPI_COMM_WORLD,&status); 
                    to = *token;
                    *token = 1;
                    MPI_Send(token,1,MPI_INT,elev[0],2,MPI_COMM_WORLD);
                    people++;
                    i = 1;
                } else {
                    i = 0;
                }

                //get destination from other people and check which are going in and which are not
                for(; i < newpeople; i++){
                    MPI_Recv(token,1,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
                    if(to > floor && *token > floor){
                        if(*token > to)
                            to = *token;
                        *token = 1;
                        MPI_Send(token,1,MPI_INT,status.MPI_SOURCE,2,MPI_COMM_WORLD);
                        people++;
                    } else if(to < floor && *token < floor){
                        if(*token < to)
                            to = *token;
                        *token = 1;
                        MPI_Send(token,1,MPI_INT,status.MPI_SOURCE,2,MPI_COMM_WORLD);
                        people++;
                    } else {
                        *token = 0;
                        MPI_Send(token,1,MPI_INT,status.MPI_SOURCE,2,MPI_COMM_WORLD);
                    }
                }
            }
        } else {
            floor = *token;

            //inform of arrival
            if(!out && in && floor == to){
                printf("P%d - OUT - F%d\n", rank, floor);
                in = false;
                out = true;
                *token = 1;
                MPI_Send(token,1,MPI_INT,0,3,MPI_COMM_WORLD);
            } else {
                *token = 0;
                MPI_Send(token,1,MPI_INT,0,3,MPI_COMM_WORLD);
            }

            if(!out && !in && floor == from){
                //inform elevator that these people are at that floor (will only go in the ones that are going the same way as the first person)
                *token = 1;
                MPI_Send(token,1,MPI_INT,0,0,MPI_COMM_WORLD);
                    
                //send destinations
                *token = to;
                MPI_Send(token,1,MPI_INT,0,1,MPI_COMM_WORLD);

                MPI_Recv(token,1,MPI_INT,0,2,MPI_COMM_WORLD,&status); 
                if(*token == 1){
                    printf("P%d - IN - F%d\n", rank, floor);
                    in = true;
                }
            } else {
                //inform elevator that these people are not at that floor
                *token = 0;
                MPI_Send(token,1,MPI_INT,0,0,MPI_COMM_WORLD);
            }
        }
        
        if(rank == 0 && (to == -1 || to == floor))
            *token = 1;
        else 
            *token = 0;
        //if the elevator is idle, asks for a place to go, if not, informs them that it is not idle anyway
        MPI_Bcast(token,1,MPI_INT,0,MPI_COMM_WORLD);
        int idle = false;
        if(*token == 1){
            idle = true;
            print = false;
        }

        //if the elevator is idle, first process whose message is received decides the destination
        if(idle){
            if(rank == 0){
                bool notChosen = true;
                for(i = 1; i < size; i++){
                    MPI_Recv(token,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status); 
                    if(notChosen && *token != -1){
                        notChosen = false;
                        to = *token;
                    }
                }   
            } else {
                //people already in their destination send -1 and people still waiting send their location
                if(out){
                    *token = -1;
                    MPI_Send(token,1,MPI_INT,0,0,MPI_COMM_WORLD);
                } else {
                    *token = from;
                    MPI_Send(token,1,MPI_INT,0,0,MPI_COMM_WORLD);
                }
            }
        }

        //elevator moves unless it is idle (will move after another iteration)
        if(rank == 0 && !idle){
            if(to > floor)
                floor++;
            else if(to < floor)
                floor--;
        }

        sleep(1);
    }

    MPI_Finalize(); 
    return(0); 
}