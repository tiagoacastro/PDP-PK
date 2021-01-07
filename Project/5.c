#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <omp.h>

/*
--------------------------------------------------------------------------------------
Commands to run the program:
    • gcc -o 5 -fopenmp 5.c
    • ./5 <number of lines in system and x variables>

Number of threads is editable in the macro THREADS.
--------------------------------------------------------------------------------------
*/

//number of threads
#define THREADS 4
//max and min number for the randomizer that generates 'a' values (a in [-LIM_A, LIM_A]\{0})
#define LIM_A 5
//max and min number for the randomizer that generates 'b' values (b in [-LIM_B, LIM_B]\{0})
#define LIM_B 10

omp_lock_t lock;

int main(int argc, char **argv)
{
    int n, i, j;

    //Input check
	if(argc != 2)
        return(-1);
    else
        sscanf (argv[1],"%d",&n);

	omp_init_lock(&lock);
	omp_set_num_threads(THREADS);

    //initiate array containing values o the 'x' variables
    float xs[n], sum;
    int a, b;
    char str[n*25], str2[25];

    //mark all values as FLT_MAX so we know they aren't calculated yet
    #pragma omp parallel for shared(i)
    for (i = 0; i < n; i++)
        xs[i] = FLT_MAX;
    
    //iterate all the lines of the system
    #pragma omp parallel for shared(i, xs, n) private(j, sum, a, b, str, str2)
    for (i = 0; i < n; i++) {
        //reset and generate 'b' value for this line of the system (b in [-LIM_B, LIM_B]\{0})
        b=0;
        while(b==0)
            b = (rand() % (2*LIM_B)) - LIM_B;
        //set sum of x values already calculated to 0
        sum = 0;
        //reset string for atomic printf
        sprintf(str, "thread %d | line %d --- ", omp_get_thread_num(), i+1);
        //calculate sum of x values already calculated
        for (j = 0; j < i; j++) {
            while(xs[j] == FLT_MAX); 
            //reset and generate 'a' value for this x (a in [-LIM_A, LIM_A]\{0})
            a=0;
            while(a==0)
                a = (rand() % (2*LIM_A)) - LIM_A;
            sum += a * xs[j];

            sprintf(str2,"%d*%f + ", a, xs[j]);
            strcat(str, str2);
        }
        //reset and generate 'a' value for the incognito 'x' (a in [-LIM_A, LIM_A]\{0})
        a=0;
        while(a==0)
            a = (rand() % (2*LIM_A)) - LIM_A;
        //calculate value for the incognito 'x'
        xs[i] = (b - sum) / a;

        //print the whole line atomically
        sprintf(str2,"%d*x = %d --- x = %f\n", a, b, xs[i]);
        strcat(str, str2);
        printf("%s", str);
    }
}