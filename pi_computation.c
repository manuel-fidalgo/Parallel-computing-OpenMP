#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NUM_STEPS 100000 //MAX_INT
double current_time();

//pi calculus
int main ()
{

	int n_threads;
	int NUM_MAX_THREADS = 16;
	for (n_threads = 2; n_threads <= NUM_MAX_THREADS; n_threads =+ 2) //2 4 6 8 10 12 14 16
	{
		int i;
		double x,sum,step,aux,pi;
		double PI25DT = 3.141592653589793238462643;
		double init_time, end_time = 0; 
		x = 0.0; sum = 0.0; aux = 0.0; pi = 0.0;
		step = 1.0/(double) NUM_STEPS;

		init_time = current_time();
		#pragma omp parallel private(i,x,aux) shared(sum) num_threads(n_threads)
		{
			#pragma omp for schedule(static) omp_nu
			for(i=0; i<NUM_STEPS; i++){

				x=(i+0.5)*step;
				aux=4.0/(1.0+x*x);

			#pragma omp critical
				sum = sum + aux;
			}
		}
		pi=step*sum;
		end_time = current_time();
		printf("PI result->[%f], Error->[%f], Time->[%f], Num threads->[%d]\n",pi,fabs(PI25DT-pi),(end_time-init_time),n_threads);
	}
	return 0;
}
double current_time(){
	struct timeval tv;
	double t;

	gettimeofday(&tv, NULL);

	t = (double)tv.tv_sec;
	t += ((double)tv.tv_usec)/1000000.0;

	return t;
}