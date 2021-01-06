#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>  
#include <math.h>
#include "tmeas.h" 
/*
--------------------------------------------------------------------------------------
Commands to run the program:
    • gcc -g -Wall tmeas.c -o 3 3.c -lpthread -lm
    • ./3 <value of x> <limit of somatory (n)>
--------------------------------------------------------------------------------------
*/

//value of x
int x;
//limit of somatory
int n;
//mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//sum
float sum;
//number of threads stored globaly for access in thread
int nthreads;

//function for calculating each interval value
void* thread(void* number) {
    int nthread = (int) number;
    //starting point for the thread
    int start = nthread*n/nthreads;
    //ending point for the thread
    int end = (nthread+1)*n/nthreads;
    //local sum
    float localSum = 0;

    printf("Thread %d calculating from %d to %d\n", nthread, start, end-1);
    
    //iterate all numbers in the inteval and add to the local sum of the interval
    for (int i = start; i < end; i++){
        long double exp = pow(x,i);
        long long int fact = 1;
        for (int j = 1; j <= i; j++) {
            if(j < 21)
                //Max number factorized that long long int can handle is 20
                fact *= j;                                                
            else
                //After that, division passes on to exp for a better result
                exp /= j;                                                 
        }

        //calculate the value of the sum of the interval
        localSum += (float) exp/fact;
    }

    //add local sum to global sum
    pthread_mutex_lock(&mutex);
        sum += localSum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int execution(){
    pthread_t id[nthreads];
    sum = 0;

    //start time count
    tstart();

    //thread setup
    for (int i=0; i < nthreads; i++) {
        errno = pthread_create(&id[i], NULL, thread, (void*)(i));
        if (errno) {
            perror("Error while creating the thread\n");
            return EXIT_FAILURE;
        }   
    }

    //thread joins
    for (int i=0; i < nthreads; i++) {
        if( pthread_join (id[i], NULL ) ) {
            printf("Error while joining the thread\n");
            abort();
        }
    }

    //stop time count
    float ts = tstop();

    printf("Result: %f\nTime: %f\n\n", sum, ts);

    return 0;
}

int main(int argc, char **argv)
{
    //Input check
	if(argc != 3)
        return(-1);
    else{
        sscanf (argv[1],"%d",&x);
        sscanf (argv[2],"%d",&n);
    }

    //----------------------- 1 thread -----------------------
    if(n < 1){
        printf("limit of somatory is too small to run program with %d threads\n", n);
    } else {
        nthreads = 1;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 2 threads -----------------------
    if(n < 2){
        printf("limit of somatory is too small to run program with %d threads\n", n);
    } else {
        nthreads = 2;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 4 threads -----------------------
    if(n < 4){
        printf("limit of somatory is too small to run program with %d threads\n", n);
    } else {
        nthreads = 4;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 6 threads -----------------------
    if(n < 6){
        printf("limit of somatory is too small to run program with %d threads\n", n);
    } else {
        nthreads = 6;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 8 threads -----------------------
    if(n < 8){
        printf("limit of somatory is too small to run program with %d threads\n", n);
    } else {
        nthreads = 8;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;  
}