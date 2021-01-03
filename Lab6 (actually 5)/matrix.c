#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#define N 1000

int test(double **Matrix1, double **Matrix2, double **Matrix3)
{
   int i,j,k,chk=0;
   double tmp,**Mtest;
    
   for (i = 0; i < N; i++) 
   {
      for (j = 0; j < N; j++)
      {
         tmp = 0;
         for (k = 0; k < N; k++)
         {
            tmp += Matrix1[i][k] * Matrix2[k][j];
         }
         if(Matrix3[i][j]!=tmp)
         {
            printf("Error in element %d,%d!\n",i,j);
            chk=1;	  
         }
      }
   }
   return chk;
}

int main(void) 
{
   double **A,**B,**C;
   int i, j, k, chk;
   double start,stop;
   double sum;
   A=(double **)malloc(N*sizeof(double *));
   B=(double **)malloc(N*sizeof(double *));
   C=(double **)malloc(N*sizeof(double *));
   for(i=0;i<N;i++)
   {
      A[i]=(double *)malloc(N*sizeof(double));
      B[i]=(double *)malloc(N*sizeof(double));
      C[i]=(double *)malloc(N*sizeof(double));
   }
   srand(time(NULL));
   for(i=0;i<N;i++)
      for(j=0;j<N;j++)
      {
	      A[i][j]=rand()%1001/1000.*100;
         B[i][j]=rand()%1001/1000.*100;
      }

   omp_set_num_threads(4);
   start=omp_get_wtime();

   //fragment for paralelization
   #pragma omp parallel for collapse(2) shared(A,B,C) private(sum,i,j,k)      //case 1 & 2 & 3
   for (i = 0; i < N; i++) 
   {
      for (j = 0; j < N; j++)
      {
         sum = 0;
         //#pragma omp parallel for shared(A,B,C,i,j,sum) private(k)            //case 2
         //#pragma omp parallel for reduction(+:sum)                            //case 3
         for (k = 0; k < N; k++)
         {  
            sum += A[i][k] * B[k][j];
         }
         C[i][j] = sum;
      }
   }
    
   stop=omp_get_wtime();
   printf("Time of matrix multiplification = %lf\n",stop-start); 
   
   chk=test(A,B,C);
   if(chk==1)
	printf("Program exited with errors\n");
   return 0;
}
