#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define EXIT_BAD_ALLOC 1

#define DEBUG 0 //!0 true; 0 false

//Headers
double current_time();
void partialAddition(double * subvector, int subvector_size);
double totalAddition(double * vector, int subvector_size, int nTHREADS);
double addition(double * vector, int vector_size, int nTHREADS);
void initVector(double * vector, int size, double * no_parallel_sum);

//sum vector
int main ()
{
	int vector_size = 10000000;
	printf("VECTOR_SIZE-> %d\n",vector_size);
	int n_threads;
	int NUM_MAX_THREADS = 16;
	for (n_threads = 2; n_threads <= NUM_MAX_THREADS; n_threads += 2) //2 4 6 8 10 12 14 16
	{
		srand(time(NULL));
		double init_time, end_time = 0; 
		int nthreads, tid, nprocs, i;

		double no_parallel_sum , parallel_sum = 0.0;
		double * vector;
		

		vector = (double *) malloc(vector_size * sizeof(double));
		initVector(vector,vector_size, &no_parallel_sum);

		init_time = current_time();
		parallel_sum = addition(vector,vector_size,n_threads);
		end_time = current_time();

		printf("Num threads->[%d], %f \n",n_threads,(end_time-init_time));

		free(vector);
	}
	return EXIT_SUCCESS;
}

//Creates the vector and fills it with random values [1,5] pointer to pointer is required for malloc in a function
void initVector(double * vector, int size, double * no_parallel_sum){

	int random, i = 0;
	double accum = 0.0;
	for (i=0; i<size; i++){
		random = rand()%5 + 1;
		vector[i] = random;
		accum = accum+random;
	}
	*no_parallel_sum = accum;

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
double totalAddition(double * vector, int subvector_size, int nTHREADS){
	int i;
	double s=0;

	for(i=0; i<(subvector_size*nTHREADS); i+=subvector_size){
		s+=vector[i];
	}
	return s;
}

//Launch nTHREADS threads with nTHREADS subvector, each thread will compute one subvector.
double addition(double * vector, int vector_size, int nTHREADS){

	int i;
	int subvector_size = (int) vector_size/nTHREADS;
	int last_subvector_size = subvector_size + (vector_size%nTHREADS); //if vector_size is not divisible between nTHREADS

	if(DEBUG) printf("vector base pointer->[%p]\n",vector);
	if(DEBUG) printf("subvector_size->%d last_subvector_size->%d\n",subvector_size,last_subvector_size);

	#pragma omp parallel for num_threads(nTHREADS) //parallels the for loop with nTHREADS threads
	for (i=0; i<nTHREADS; i++){

		if(i==(nTHREADS-1)){
			partialAddition(vector+(i*subvector_size),last_subvector_size);
		}
		else{
			partialAddition(vector+(i*subvector_size),subvector_size);
		}
		if(DEBUG) printf("I'm thread nº %d, I process the section %d.\n", omp_get_thread_num(),i);

	}
	return totalAddition(vector,subvector_size,nTHREADS);
}
double current_time(){
	struct timeval tv;
	double t;

	gettimeofday(&tv, NULL);

	t = (double)tv.tv_sec;
	t += ((double)tv.tv_usec)/1000000.0;

	return t;
}