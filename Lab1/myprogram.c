#include "tmeas.h"
#include <stdio.h>

//results.txt contains all run data with the number of iterations, arithmetic loop time and printing loop time

int main(int argc, char **argv){

    //Input check
    int it = 100000;                              //default number of iterations is 100000
    if(argc > 2)
        return(-1);
    else if(argc == 2)
        sscanf (argv[1],"%d",&it);

    //File opening
    FILE *f = fopen("results.txt", "a");
    fprintf(f, "%d ", it);

    //Arithmetic operation loop
    int n = 1;
    tstart();
    for(int i = 0; i < it; ++i){
        n = n + n;
    }
    double t1 = tstop();
    fprintf(f, "%f ", t1);

    //Printing loop
    tstart();
    for(int i = 0; i < it; ++i){
        printf(".");
    }
    double t2 = tstop();
    fprintf(f, "%f\n", t2);
    fclose(f);

    printf("\nArithmetic operation loop time for %d iterations was %f seconds\n", it, t1);
    printf("Printing loop time for %d iterations was %f seconds\n", it, t2);
}