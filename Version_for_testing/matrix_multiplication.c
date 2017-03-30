#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double current_time();
void fillMatrix(double ** M, int rows, int cols);
void linear_multiplication(double ** MULT,double ** M1, double ** M2,int m, int n, int p);
void pararell_multiplication(double ** MULT, double ** M1, double ** M2,int m, int n, int p,int num_threads);
double addictionProduct(double ** MULT, double ** M1, double ** M2, int i, int j, int n);

//matrix multiplication
int main ()
{
	int m,n,p = 0;
	m=1000;n=1000;p=1000; //1000x1000 matrix
	printf("MATRIX_SIZE-> (%d*%d)\n",m,n);
	int n_threads;
	int NUM_MAX_THREADS = 16;
	for (n_threads = 2; n_threads <= NUM_MAX_THREADS; n_threads +=2) //2 4 6 8 10 12 14 16
	{

		int i;
		double init_time, end_time;
		srand(time(NULL));

	/*********************************
	* M1(m,n) X M2(n,p) = MULT(m,p) *
	*********************************/

		double ** M1; //(m,n)
		double ** M2; //(n,p)
		double ** MULT;

		M1 = (double **)malloc(m*sizeof(double *));
		for (i = 0; i < m; ++i)
			M1[i] = (double *) malloc(n*sizeof(double));

		M2 = (double **)malloc(n*sizeof(double *));
		for (i = 0; i < n; ++i)
			M2[i] = (double *) malloc(p*sizeof(double));

		MULT = (double **)malloc(m*sizeof(double *));
		for (i = 0; i < m; ++i)
			MULT[i] = (double *) malloc(p*sizeof(double));


		fillMatrix(M1,m,n);
		fillMatrix(M2,n,p);

		init_time = current_time();
		pararell_multiplication(MULT,M1,M2,m,n,p,n_threads);
		end_time = current_time();
		printf("Num threads->[%d], %f \n",n_threads,(end_time-init_time));

		//memory unallocation
		for (i = 0; i < m; ++i){ free(M1[i]); free(M2[i]); free(MULT[i]); } 

		free(M1);
		free(M2);
		free(MULT);

	}
	return 0;
}

//M1(m,n) X M2(n,p) = MULT(m,p)
void pararell_multiplication(double ** MULT, double ** M1, double ** M2,int m, int n, int p, int num_threads){
	//i(iterates in M1 as row), j(iterates in M2 as column)
	int i, j = 0;

	//this parallels the nested fors with one thread for calculate each element in the final matrix
	#pragma omp parallel for num_threads(num_threads) collapse(2)
	for (i = 0; i < m; ++i) //row nº i€[0,m)
	{
		for (j = 0; j < p; ++j) //column nº j€[0,p)
		{
			addictionProduct(MULT,M1,M2,i,j,n);
		}
	}
}

//Sum and multiply all the elemetst in the row i and the column j, later it stores in the
double addictionProduct(double ** MULT, double ** M1, double ** M2, int i, int j, int n){

	int k;
	double aux=0;

	for (k = 0; k < n; k++)
	{
		aux += M1[i][k] * M2[k][j];
	}
	MULT[i][j] = aux;
}

void fillMatrix(double ** M, int rows, int cols){
	int i,j;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			M[i][j] = rand()%5+1;
			// printf("%.2f ", M[i][j]);
		}
		// printf("\n");
	}
	// printf("\n-----\n");
}
double current_time(){
	struct timeval tv;
	double t;

	gettimeofday(&tv, NULL);

	t = (double)tv.tv_sec;
	t += ((double)tv.tv_usec)/1000000.0;

	return t;
}
/*
void linear_multiplication(double ** MULT, double ** M1, double ** M2,int m, int n, int p){
	//i(iterates in M1 as rows), j(iterates in M2 as columns)
	int i, j, k = 0;
	double aux = 0;

	for (i = 0; i < m; ++i)
	{
		//row nº i€[0,m)
		for (j = 0; j < p; ++j)
		{
			//column nº j€[0,p)
			for (k = 0; k < n; ++k)
			{
				aux += M1[i][k] * M2[k][j];
			}
			MULT[i][j] = aux;
			printf("%.2f ", MULT[i][j]);
			aux = 0;
		}
		printf("\n");
	}
}
*/
