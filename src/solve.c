#include "matrix.h"
#include "print.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

//Найти корни и записать в answer
void solve(const int n, const int m, double* A, double* b, double* X)
{
	//итераторы
	int i, j, k;
	// размер блока av * ah
	int av, ah;
	// количество блоков размера m
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;
	//вспомогательные матрицы
	double V1[m * m], V2[m * m], Vmax[m * m];
	//максимальная норма
	double max;
	//указатель на текущий блок
	double* pa;
	//идем по блоковым столбцам
	// for(j = 0; j * m < n; j++) {
	// 	ah = j < k ? m : l;
	// 	pa = A(j,j);
	// 	// A_{j,j} --> V1
	// 	for()
	// }
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

