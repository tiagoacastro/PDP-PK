#include "tmeas.h"
#include <stdio.h>		/* need for fflush() */
#include <stdlib.h>		/* need for exit() */
#include <pthread.h>

#define WAIT 1000000

void* my_pthread_function( void *ptr );

main(int argc, char **argv)
{	
	//Input check
	int nThreads = 20;  
	if(argc > 2)
        return(-1);
    else if(argc == 2)
        sscanf (argv[1],"%d",&nThreads);
	
	//File opening
    FILE *f = fopen("results.txt", "a");
    fprintf(f, "%d ", nThreads);

	pthread_t thread_array[nThreads];
     	int i, thr;

	tstart();
	for( i = 0; i < nThreads; i++ )
	{
		thr = pthread_create( &thread_array[i], NULL, my_pthread_function, (void *)i );
		if( thr ) {
			fprintf( stderr, "Error: return code from pthread_create() - %d", thr);
			exit(-1);
		}
	}

	/* Wait till threads are complete before main continues. Unless we  */
     	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	for( i = 0; i < nThreads; i++ )	
		pthread_join( thread_array[i], NULL);

	float t = tstop();
	printf("Time of the execution for %d threads was %f seconds\n", nThreads, t);
	fprintf(f, "%f\n", t);
    fclose(f);
	
	return EXIT_SUCCESS;    
}

void* my_pthread_function( void *thr_data )
{
	int i,id;
	id=(int)thr_data;
	printf("I am %d thread\n", id);	
	//busy waiting
	while(i<WAIT) i++;	
}
