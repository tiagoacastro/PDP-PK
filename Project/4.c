#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>  
#include <math.h>
/*
--------------------------------------------------------------------------------------
Commands to run the program:
    • gcc -g -Wall -o 4 4.c -lpthread -lm
    • ./4 <number of elements of the sum>

Number of processes is editable in the macro P.
--------------------------------------------------------------------------------------
*/

#define P 10

//number of elements of the sum
int n;
//mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//sum
float sum;

//function for calculating each interval value
void* thread(void* number) {
    int nthread = (int) number;
    //starting point for the thread
    int start = nthread*n/P+1;
    //ending point for the thread
    int end = (nthread+1)*n/P;
    //local sum
    float localSum = 0;

    //iterate all numbers in the inteval and add to the local sum
    for (int i = start; i <= end; i++)
        localSum += 1./i;

    printf("Thread %d calculated from %d to %d: %f\n", nthread, start, end, localSum);

    //add local sum to global sum
    pthread_mutex_lock(&mutex);
        sum += localSum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int execution(){
    pthread_t id[P];
    sum = 0;

    //thread setup
    for (int i=0; i < P; i++) {
        errno = pthread_create(&id[i], NULL, thread, (void*)(i));
        if (errno) {
            perror("Error while creating the thread\n");
            return EXIT_FAILURE;
        }   
    }

    //thread joins
    for (int i=0; i < P; i++) {
        if( pthread_join (id[i], NULL ) ) {
            printf("Error while joining the thread\n");
            abort();
        }
    }

    printf("Sum: %f\nln= %f\nResult: %f\n", sum, log(n), sum - log(n));

    return 0;
}

int main(int argc, char **argv)
{
    //Input check
	if(argc != 2)
        return(-1);
    else
        sscanf (argv[1],"%d",&n);

    if(execution() == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;  
}