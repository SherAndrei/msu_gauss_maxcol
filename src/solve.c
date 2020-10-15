#include "matrix.h"
#include "print.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

//Найти корни и записать в answer
void solve(const int n, const int m, double* A, double* b, double* X)
{
	(void) n;
	(void) m;
	(void) A;
	(void) b;
	(void) X;
}

double  norm(const double* vector, const int length)
{
	(void) vector;
	(void) length;
    return 0.;
}

//Норма невязки
double residual(const int dim, const double* matrix,const double* r_part,const double* answer)
{
	(void) dim;
	(void) matrix;
	(void) r_part;
	(void) answer;
	return 0.;
}

//Норма разности с ответом
double difference(const int dim, double* answer)
{
	(void) dim;
	(void) answer;
	
    return 0.;
}

