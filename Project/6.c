#include <stdio.h>
#include <omp.h>

/*
--------------------------------------------------------------------------------------
Commands to run the program:
    • gcc -o 6 -fopenmp 6.c
    • ./6 <number of natural numbers>

Number of threads is editable in the macro THREADS.
--------------------------------------------------------------------------------------
*/

//number of threads
#define THREADS 4

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

    //number of numbers to analyze incremented by 1 so each position in the array equals the number in the position
    n++;
    //array containing all the numbers being that values can be 1 (prime) and 0 (not prime)
    int primes[n];
    //set 0 to 0 as 0 is not prime and will not be contemplated
    primes[0] = 0;

    //all numbers start as prime except for 0
    #pragma omp parallel for shared(i)
    for (i = 1; i < n; i++)
        primes[i] = 1;

    //iterate all the numbers set to 1 starting in 2 (because primes numbers are only dividable by 1 and themselves)
    #pragma omp parallel for shared(i, primes, n) private(j)
    for (i = 2; i < n; i++) {
        //if it is 1 then tag its multiples as 0
        if(primes[i]){
            //start in the first multiple and go along
            for (j = i*2; j <  n; j += i) {
                //set to 0 the multiple
                primes[j] = 0;
            }
        }
    }

    //print the prime numbers
    for (i = 0; i < n; i++) {
        if(primes[i])
            printf("%d\n", i);
    }
}