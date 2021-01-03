#include<stdio.h>
#include<omp.h>

#define CHUNK_SIZE 3        //to use default simply remove CHUNK_SIZE from #pragma line
#define THREADS 4
#define TYPE static     //dynamic or static
#define ITERATIONS 15       //Change to 15.000 for exercise 5

int main()
{
    //float ts = omp_get_wtime();       //present for exercise 5
    omp_set_num_threads(THREADS);
    int a = 0;
    #pragma omp parallel for schedule(TYPE, CHUNK_SIZE)
    for(int i=0;i<ITERATIONS;i++)
    {
        a+=1;
        printf("Thread %d, index %d, value of a is %d\n",omp_get_thread_num(),i, a);        //disabled for exercise 5
    }
    //float t = omp_get_wtime() - ts;       //present for exercise 5
    //printf("Execution time = %f\n", t);       //present for exercise 5
    printf("After loop a=%d\n",a);        
}