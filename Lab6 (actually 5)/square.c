#include<stdio.h>
#include<omp.h>

#define N 500

//omp_lock_t lock;                                                            //test case 2

int main()
{
   int square=0, number, i;
	
   printf("Insert number:\n");
   scanf("%d",&number);

   float ts = omp_get_wtime(); 

   //how to parallelize it with good final result?
   #pragma omp parallel for reduction(+:square)                             //test case 1
   for(i=0;i<N;i++)
   {
      //omp_set_lock(&lock);                                                  //test case 2
      //#pragma omp atomic                                                    //test case 3
      //#pragma omp critical                                                  //test case 4
	   square += number * number;
      //omp_unset_lock(&lock);                                                //test case 2
   }

   float t = omp_get_wtime() - ts;     
   printf("Execution time = %f\n", t); 
   
   printf("Square=%d\n",square);
}
