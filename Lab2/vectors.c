#include <stdio.h>
#include <stdlib.h>
#include "tmeas.h"
#include <pthread.h>

/* define structure of thread's data */
typedef struct {
	int id;
	int *a;
	int *b;
	int *c;
} vectors;

vectors vec;
int step;

void add(void *i)
{
	printf("Aggregating %d to %d\n", i, i+step);
	for(int it=(int)i;it<(int)i+step;it++)
		vec.c[it]=vec.a[it]+vec.b[it];
}

int main(int argc, char **argv)
{
	//Input check
	int nThreads = 4;  
	int vSize= 12;  
	if(argc > 3)
        return(-1);
    else if(argc == 3){
		sscanf (argv[1],"%d",&nThreads);
		sscanf (argv[2],"%d",&vSize);
	}
	step = vSize/nThreads;

	//File opening
    FILE *f = fopen("results_vector.txt", "a");
    fprintf(f, "%d ", nThreads);
	fprintf(f, "%d ", vSize);

	int i;
	double t;
	
	vec.a=(int *)malloc(sizeof(int)*vSize);
	vec.b=(int *)malloc(sizeof(int)*vSize);
	vec.c=(int *)malloc(sizeof(int)*vSize);
	
	srand(time(NULL));			//random numbers generator initialization;
	
	for(i=0;i<vSize;i++)
	{
		vec.a[i]=rand()%1001;		//random number generator
		vec.b[i]=rand()%1001;
		printf("vec.a[%d]=%d,vec.b[%d]=%d\n",i,vec.a[i],i,vec.b[i]);
	}

	pthread_t thread_array[nThreads];
	int thr;
	tstart();
	for(i=0;i<nThreads;i++)
	{
		int par = i*step;
		thr = pthread_create( &thread_array[i], NULL, add, (void *)par);
		if(thr) {
			fprintf( stderr, "Error: return code from pthread_create() - %d", thr);
			exit(-1);
		}
	}
	//join
	for( i = 0; i < nThreads; i++ )	
		pthread_join( thread_array[i], NULL);
	t=tstop();

	for(i=0;i<vSize;i++)
		printf("vec.c[%d]=%d\n",i,vec.c[i]);
	printf("time=%lf\n",t);
	fprintf(f, "%f\n", t);
    fclose(f);

	return 0;
}
	
	
