#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_STEPS 2147483647 //MAX_INT
//pi calculus
int main ()
{
	int i;
	double x,sum,step,aux,pi;
	x = 0.0; sum = 0.0; aux = 0.0; pi = 0.0;
	step = 1.0/(double) NUM_STEPS;

	#pragma omp parallel private(i,x,aux) shared(sum) 
	{
		#pragma omp for schedule(static) 
		for(i=0; i<NUM_STEPS; i++){

			x=(i+0.5)*step;
			aux=4.0/(1.0+x*x);

			#pragma omp critical
			sum = sum + aux;
		}
	}
	pi=step*sum;
	printf("Numero PI->%f\n",pi);
	return 0;
}
