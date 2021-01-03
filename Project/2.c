#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>  
#include "tmeas.h" 
/*
--------------------------------------------------------------------------------------
Commands to run the program:
    • gcc -g -Wall tmeas.c -o 2 2.c -lpthread
    • ./2 <number of subintervals of integration>
--------------------------------------------------------------------------------------
*/

#define A 0
#define B 1

//number of subintervals
int n;
//mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//sum
float sum;
//number of threads stored globally for access in thread
int nthreads;
//length of each subinterval
float step;

//function
double f(double x){
    return 1./(1+x*x);
}

//trapezoid function
double t(){
    return step*sum/2.;
}

//function for calculating each subinterval value
void* thread(void* number) {
    int nthread = (int) number;
    //starting point for the thread
    int start = nthread*n/nthreads;
    //ending point for the thread
    int end = (nthread+1)*n/nthreads;
    //local sum
    float localSum = 0;

    for (int i = start; i < end; i++){
        //first thread's first computation isn't multiplied by 2
        if(i == 0)
            localSum += f(A);
        else
            localSum += 2*f(A+step*i);

        //printf("%f - %f\n", A+step*i, f(A+step*i));
    }

    //last thread computes also the last one
    if(end == n)
        localSum += f(B);

    pthread_mutex_lock(&mutex);
        sum += localSum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int execution(){
    pthread_t id[nthreads];
    sum = 0;

    tstart();

    //thread setup
    for (int i=0; i < nthreads; i++) {
        errno = pthread_create(&id[i], NULL, thread, (void*)(i));
        if (errno) {
            perror("Error while creating the thread");
            return EXIT_FAILURE;
        }   
    }

    //thread joins
    for (int i=0; i < nthreads; i++) {
        if( pthread_join (id[i], NULL ) ) {
            printf("Error while joining the thread");
            abort();
        }
    }

    float ts = tstop();

    printf("Result: %f\nTime: %f\n\n", t(), ts);

    return 0;
}

int main(int argc, char **argv)
{
    //Input check
	if(argc != 2)
        return(-1);
    else
        sscanf (argv[1],"%d",&n);

    step = 1./n;

    //----------------------- 1 thread -----------------------
    if(n < 1){
        printf("Number of subintervals is too small to run program with %d threads", n);
    } else {
        nthreads = 1;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 2 threads -----------------------
    if(n < 2){
        printf("Number of subintervals is too small to run program with %d threads", n);
    } else {
        nthreads = 2;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 4 threads -----------------------
    if(n < 4){
        printf("Number of subintervals is too small to run program with %d threads", n);
    } else {
        nthreads = 4;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 6 threads -----------------------
    if(n < 6){
        printf("Number of subintervals is too small to run program with %d threads", n);
    } else {
        nthreads = 6;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    //----------------------- 8 threads -----------------------
    if(n < 8){
        printf("Number of subintervals is too small to run program with %d threads", n);
    } else {
        nthreads = 8;
        if(execution() == EXIT_FAILURE)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;  
}