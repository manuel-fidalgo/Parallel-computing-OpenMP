#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define EXIT_BAD_ALLOC 1


//Headers
void sumaparcial(double *a, int n);
double sumatotal(double *a, int n, int nTHREADS);
double suma(double *a, int n, int nTHREADS);
void initVector(double * vector, int size, int * no_parallel_sum);

//sum vector
int main ()
{	
	srand(time(NULL));

	int nthreads, tid, nprocs, vector_size, i;
	double no_parallel_sum = 0.0;
	double * vector;

	nthreads = 4; //Sets here the number of threads
	nprocs = omp_get_num_procs();

	printf("Number of procs in use->[%d]\n",nprocs);
	printf("Number of threads will be launch->[%d]\n",nthreads);
	resultadosuma = suma(vector,tam,nthreads);
	printf("Resultado sin paralelo->[%f], resultado paralelo->[%f]\n",no_parallel_sum,resultadosuma);

	return 0;
}
//Creates the vector and fills it with random values [1,5]
void initVector(double * vector, int size, int * no_parallel_sum){

	vector = (double *) malloc(size*sizeof(double));
	if(vector==NULL){
		printf("[ERROR]: bad memory allocation\n");
		exit(EXIT_BAD_ALLOC);
	}else{
		for (i=0; i<tam; i++){
			random = rand()%5 + 1;
			vector[i] = random;
			*no_parallel_sum = *no_parallel_sum + random;
		}
	}
}

//Un hilo sumara todos los elementos de cada uno de los subvectores y guarda el resultado en la primera posiion
void sumaparcial(double *a, int n){
	int i;
	double s=0;
	for(i=0;i<n;i++)
		s+=a[i];
	a[0]=s;
}
//Suma el primer elementos de cada uno de los subvectores que han sido sumados por cada uni de los diferentes hilos
double sumatotal(double * a, int n, int nTHREADS){
	int i;
	double s=0;
	for(i=0;i<n;i+=n/nTHREADS)
		s+=a[i];
	return s;
}
//This method launch THREAD
double suma(double * a, int n, int nTHREADS){
	
	int i;
	#pragma omp parallel for //parallels the for loop
	for (i=0;i<nTHREADS;i++){
		sumaparcial(a + (i*(n/nTHREADS)) , n/nTHREADS);
		printf("I'm thread nº %d, I process the section %d.\n", omp_get_thread_num(),i);	
	}
	return sumatotal(a,n,THREADS);
}
