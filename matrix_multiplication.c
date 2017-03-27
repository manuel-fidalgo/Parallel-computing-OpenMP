#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void fillMatrix();

//matrix multiplication
int main ()
{
	int i;

	//M1(m,n) X M2(n,p) = MULT(m,p)
	double ** M1; //(m,n)
	double ** M2; //(n,p)
	double ** MULT;

	int m,n,p = 0;
	m=5;n=4;p=5;

	M1 = (double **)malloc(m*sizeof(double *));
	for (i = 0; i < m; ++i)
	{
		M1[i] = (double *) malloc(n*sizeof(double));
	}

	M2 = (double **)malloc(n*sizeof(double *));
	for (i = 0; i < n; ++i)
	{
		M2[i] = (double *) malloc(p*sizeof(double));
	}

	MULT = (double **)malloc(m*sizeof(double *));
	for (i = 0; i < m; ++i)
	{
		MULT[i] = (double *) malloc(p*sizeof(double));
	}


	fillMatrix(M1,m,n);
	fillMatrix(M2,n,p);

	multiplication(MULT,M1,M2,m,n,p);

	return 0;
}
//M1(m,n) X M2(n,p) = MULT(m,p)
void multiplication(int ** MULT,int ** M1, int ** M2,int m, int n, int p){
	
}

void fillMatrix(double ** M, int rows, int cols){
	int i,j;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			M[i][j] = i+j;
			printf("%.2f ", M[i][j]);
		}
		printf("\n");
	}
	printf("\n-----\n");
}
