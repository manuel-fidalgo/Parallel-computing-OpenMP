#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define EXIT_BAD_ALLOC 1

#define DEBUG 0 //!0 true; 0 false

//Headers
void partialAddition(double * subvector, int subvector_size);
double totalAddition(double * vector, int vector_size, int subvector_size);
double addition(double * vector, int vector_size, int nTHREADS);
void initVector(double * vector, int size, double * no_parallel_sum);

//sum vector
int main ()
{	
	srand(time(NULL));

	int nthreads, tid, nprocs, vector_size, i;

	double no_parallel_sum , parallel_sum = 0.0;
	double * vector;
	vector_size = 102;

	nthreads = 4; //Sets here the number of threads
	nprocs = omp_get_num_procs();

	vector = (double *) malloc(vector_size * sizeof(double));
	initVector(vector,vector_size,&no_parallel_sum);

	printf("Number of procs in use->[%d]\n",nprocs);
	printf("Number of threads will be launch->[%d]\n",nthreads);
	parallel_sum = addition(vector,vector_size,nthreads);
	printf("No parallel->[%f], parallel->[%f]\n",no_parallel_sum,parallel_sum);

	return EXIT_SUCCESS;
}

//Creates the vector and fills it with random values [1,5] pointer to pointer is required for malloc in a function
void initVector(double * vector, int size, double * no_parallel_sum){

	int random, i = 0;
	for (i=0; i<size; i++){
		//random = rand()%5 + 1;
		random = 1;
		vector[i] = random;
		*no_parallel_sum += random;
	}
	
}
//addition
//Un hilo sumara todos los elementos de cada uno de los subvectores y guarda el resultado en la primera posiion
void partialAddition(double * subvector, int subvector_size){
	
	if(DEBUG) printf("Subvector base pointer->[%p]\n",subvector);

	int i;
	double s=0;

	for(i=0;i<subvector_size;i++)
		s+=subvector[i];

	subvector[0] = s; //Store the result in the first position of the subvector
}

//Adds and returns the first element of each subvector.
double totalAddition(double * vector, int vector_size, int subvector_size){
	int i;
	double s=0;

	for(i=0; i<vector_size; i+=subvector_size)
		s+=vector[i];
	return s;
}

//BUG -> if vector_size%nTHREADS !=0

//Launch nTHREADS threads with nTHREADS subvector, each thread will compute one subvector.
double addition(double * vector, int vector_size, int nTHREADS){
	
	int i;
	int subvector_size = (int) vector_size/nTHREADS;
	int last_subvector_size = subvector_size + (vector_size%nTHREADS); //if vector_size is not divisible between nTHREADS

	if(DEBUG) printf("vector base pointer->[%p]\n",vector);
	printf("subvector_size->%d last_subvector_size->%d\n",subvector_size,last_subvector_size);

	#pragma omp parallel for num_threads(nTHREADS) //parallels the for loop with nTHREADS threads
	for (i=0; i<nTHREADS; i++){

		if(i==(nTHREADS-1))
			partialAddition(vector+(i*subvector_size),last_subvector_size);
		else
			partialAddition(vector+(i*subvector_size),subvector_size);
		printf("I'm thread nº %d, I process the section %d.\n", omp_get_thread_num(),i);	

	}
	return totalAddition(vector,vector_size,subvector_size);
}
