#include "matrix.h"
#include "error.h"
#include "print.h"
#include "multiply.h"
#include "gauss_inverse.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// столбцовая норма матрицы
double  norm(const double* A, const int n)
{
	int i, j;
	double max = 0, current;
	for(i = 0; i < n; i++) {
		current = 0.;
		for(j = 0; j < n; j++) {
			current += fabs(A[i * n + j]);
		}
		if(current > max)
			max = current;
	}
    return max;
}

//Поменять местами элемент lhs и rhs
static void swap(double* lhs, double* rhs) 
{
    double temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

// поменять местами блок lhs и rhs
static void swap_block(double * lhs, double *rhs, int m)
{
	int i, j;
	for(i = 0; i < m; i++)
		for(j = 0; j < m; j++) {
			swap(&(lhs[i*m + j]), &(rhs[i*m + j]));
		}
}

// заполнить одну матрицу другой
static void cpy(double* source, int av, int ah, double *dest)
{
	int i;
	for(i = 0; i < av * ah; i++)
		dest[i] = source[i];
}

static void extract(double* lhs,  double* rhs, int v, int h)
{
	int i;
	for(i = 0; i < v * h; i++)
		lhs[i] -= rhs[i];
}

#define LOG 0
#define OK 0

//Найти корни и записать в answer
int solve(const int n, const int m, double* A, double* B, double* X)
{
	//итераторы
	int i, j, q, cnt = 0;
	// размер блока av * ah
	int av, ah, qv, qh;
	// количество блоков размера m
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;
	//вспомогательные матрицы
	double *V1, *V2, *Vmin;
	//максимальная норма
	double min = 0., temp = 0.;
	// позиции блока с макс нормой
	int min_i = 0;

	if((V1 = alloc_matrix(m, m)) == NULL) 
		return error(5);
	if((V2 = alloc_matrix(m, m)) == NULL) {
		free_matrix(V1);
		return error(5);
	}
	if((Vmin = alloc_matrix(m, m)) == NULL) {
		free_matrix(V1);
		free_matrix(V2);
		return error(5);
	}


	//идем по блоковым столбцам
	for(j = 0; j * m < n; j++) {
		// A_{j,j}
		av = ah = j < k ? m : l;
		cpy(A(j,j), av, ah, V1);
		make_identity(V2, av);
		if(gauss_inverse(V1, V2, av) == 0) {
			min   = norm(V2, av);
			min_i = j;
		} else cnt ++;
		// for j+1, j
		// if av == ah == k && l != 0 => (i + 1) * m < n
		// if av == ah == k && l == 0 => (i + 0) * m < n
		// if av == ah == l && l != 0 => (i + 0) * m < n
		// if av == ah == l && l == 0 not possible
		// for(i = j; (i + (av != l) - (l == 0)) * m < n; i++) {
		for(i = j + 1; (i + 1) * m < n; i++) {
			// pa = A(i,j);
			// A_{i,j} --> V1
			cpy(A(i,j), av, ah, V1);
			make_identity(V2, av);
			// V2 = V1^(-1)
			if(gauss_inverse(V1, V2, av) == 0) {
#if 0
				print_matrix(V1, av, av, 1, av);
				print_matrix(V2, av, av, 1, av);
#endif
				temp = norm(V2, av);
				// print_matrix(V2, av, ah, 1, av);
				if(temp < min) {
					// V2 --> Vmin
					cpy(V2, av, ah, Vmin);
					min = temp;
					min_i = i;
				}
				// min = ||Vmin||
			} else cnt++;
		}
		if(cnt == k + (j == k) - j) {
			free_matrix(V1);
			free_matrix(V2);
			free_matrix(Vmin);
			printf("Algorithm inaplicable\n");
			return -1;
		}
		// I_min_i <-> I_j
		if(min_i != j) {
			for(i = j; (i + 1)* m < n; i++)
				swap_block(A(j,i), A(min_i, i), av);
			swap(&B[min_i], &B[j]);
# if LOG
			printf("Swap %d and %d:\n", min_i, j);
			print_matrix(A, n, n, m, n);
# endif
		}
		// ||A_{j,j}|| != 0
		// A_{j,j}^(-1)*(I_j)
		qv = av;
#if OK
		make_identity(A(j,j), ah);
		for(q = j + 1; q * m < n; q++) {
#elif OK - 1 
		for(q = j; q * m < n; q++) {
#endif
			qh = q < k ? m : l;
			// A(j, q) --> V1
			cpy(A(j,q), qv, qh, V1);
			// V2 = Vmin * V1
			if(conv_basic_multiply(Vmin, av, ah, V1, qv, qh, V2) == -1)
			 printf("you are wrong at A(j, q)!\n\n");	
#if LOG
			print_matrix(V2, av, qh, 1, av);
#endif
			// V2 --> A(j, q)
			cpy(V2, av, qh, A(j, q));
		}
		// разделим правую часть
		// B --> V1
		cpy(B, qv, 1, V1);
		// V2 = Vmin * V1
		if(conv_basic_multiply(Vmin, av, ah, V1, qv, 1, V2) == -1)
			printf("you are wrong at B!\n\n");
		// V2 --> B
		cpy(V2, qv, 1, B);
#if LOG
		printf("Multiply %d line by:\n", j);
		print_matrix(Vmin, av, ah, 1, av);
		// print_matrix(A, n, n, m, n);
#endif		
		// A(j,j) = E
		// A(i, q) -= A(i, j) * A(j, q) 
		for(i = j + 1; i * m < n; i++) {
			qv = i < k ? m : l;
			// A(i,j) --> V1
			cpy(A(i,j), qv, ah, V1);
			for(q = j; q * m < n; q++) {
				qh = q < k ? m : l;
				if(conv_basic_multiply(V1, qv, ah, A(j, q), av, qh, V2) == -1)
					printf("you are wrong at A(i,j)!\n\n");
				extract(A + i * n * m + q * qh * m, V2, qv, qh);	
			}
#if LOG	
			printf("Multiply %d line by:\n", j);
			print_matrix(V2, qv, qh, 1, av);
			printf("and extract from %d line:\n", i);
			print_matrix(A, n, n, m, n);
#endif
		}
		min = 0.;
		cnt = 0;
	}
	// матрица теперь верхнедиагональная
	(void) X;

	free_matrix(V1);
	free_matrix(V2);
	free_matrix(Vmin);
	return 0;
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

