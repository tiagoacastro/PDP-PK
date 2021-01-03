#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>

static struct timespec start,stop;

void tstart(void)
{
	clock_gettime( CLOCK_REALTIME, &start);	
}

double tstop(void)
{
	clock_gettime( CLOCK_REALTIME, &stop);
	return (double)( stop.tv_sec - start.tv_sec ) + (double)( stop.tv_nsec - start.tv_nsec )/1.0e9;
}


typedef struct {
	char name[20];
	int age;
	float gpa;
	char father[20];
	char mother[20];
} Data;


int main(int argc, char **argv)
{
	int rank, size, r;
	int src, dst, tag, i;	
	MPI_Status status;
	MPI_Datatype new_type;
	MPI_Datatype type[5] = { MPI_CHAR, MPI_INT, MPI_FLOAT, MPI_CHAR, MPI_CHAR};
	int blen[5] = { 20, 1, 1, 20, 20 };
	MPI_Aint disp[5];
	MPI_Aint base, addr;
	Data tabrecord,tabrecieved;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0) 
		printf("MPI_Type_create_struct()\n");

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Get_address(&tabrecord, &base);
	MPI_Get_address(&(tabrecord.name), &addr);
	disp[0] = addr - base;
	MPI_Get_address(&(tabrecord.age), &addr);
	disp[1] = addr - base;
	MPI_Get_address(&(tabrecord.gpa), &addr);
	disp[2] = addr - base;
	MPI_Get_address(&(tabrecord.father), &addr);
	disp[3] = addr - base;
	MPI_Get_address(&(tabrecord.mother), &addr);
	disp[4] = addr - base;
	
	MPI_Type_create_struct(5, blen, disp, type, &new_type);
	MPI_Type_commit(&new_type);

	if(rank != 0)
	{
		strcpy(tabrecieved.name, "Peter");
		tabrecieved.age = 14;
		tabrecieved.gpa = 4.12;
		strcpy(tabrecieved.mother, "Lois");
		strcpy(tabrecieved.father, "Chris");
		printf("In process %d my name is %s, my age is %d, my gpa is %f, my mom is %s and my dad is %s\n",rank,tabrecieved.name,tabrecieved.age,tabrecieved.gpa,tabrecieved.mother,tabrecieved.father);		
		MPI_Recv(&tabrecieved, 1, new_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("In process %d I have received name: %s ",rank,tabrecieved.name);
		printf("and age: %d ",tabrecieved.age);
		printf("and gpa: %f ",tabrecieved.gpa);
		printf("and mother: %s ",tabrecieved.mother);
		printf("and father: %s\n",tabrecieved.father);
	}
	else
	{
		strcpy(tabrecord.name, "John");
		tabrecord.age = 51;
		tabrecord.gpa = 3.12;
		strcpy(tabrecord.mother, "Senna");
		strcpy(tabrecord.father, "Carlos");

		tstart();
		for (r=1; r<size; r++)
			MPI_Send(&tabrecord, 1, new_type, r, 0, MPI_COMM_WORLD);
		float t = tstop();   
   		printf("Execution time = %f\n", t); 
	}
	MPI_Finalize();
	return 0;
}

