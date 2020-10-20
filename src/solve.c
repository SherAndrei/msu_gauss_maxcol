#include "matrix.h"
#include "error.h"
#include "print.h"
#include "multiply.h"
#include "gauss_inverse.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// столбцовая норма матрицы
double  norm(const double* const A, const int n)
{
	int i, j;
	double max = 0, current;
	for(i = 0; i < n; i++) {
		current = 0.;
		for(j = 0; j < n; j++) {
			current += fabs(A[j * n + i]);
		}
		if(current > max)
			max = current;
	}
    return max;
}

void copy(const double* source, double* dest, const int n)
{
	int i;
	for(i = 0; i < n; i++) 
		dest[i] = source[i];
}

// сделать единичной matr
void make_identity(double* const matr, const int dim)
{
	int i, j;
	for(i = 0; i < dim; i++) 
		for(j = 0; j < dim; j++)
			matr[i*dim + j] = ( i == j );
}

void null(double* const matr, const int v, const int h)
{
    int i;
    for(i = 0; i < v * h; i++)
        matr[i] = 0.;
}

#define eps (1e-16)

//Найти корни и записать в answer
int solve(const int n, const int m, double* A, double* B, double* X)
{
	// итераторы
	int i = 0, j = 0, r = 0, q = 0;
	// вспомогательные матрицы
	double *V1, *V2, *Vmin;
	// указатель на текущий блок
	double *pa, *pb;
	// указатель на очередной блок в столбце/в строчке
	double *pi, *pj;
	// размер текущего блока av * ah
	int av = 0, ah = 0;
	// количество блоков
	const int k = n / m;
	// остаток
	const int l = n - k * m;
	// погрешность
    double ERROR;
	// double ERROR = norm(A, n) * 1e-16;
	// минимальная норма обратной матрицы
	double min = 0.;
	// строчка с минимальной матрицей
	int min_i = 0;
	// счетчик необратимых матриц в столбце
	int c = 0;
	// текущая норма
	double current = 0.;

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

	for(j = 0; j * m < n; j++)
	{
		// A_{j,j} square!
		av = ah = j < k ? m : l;
		pa = A + j * n * m + j * av * m;

		// A_{j,j} --> V1
		// V_min = (A_{j,j})^(-1)
		// min = ||V_min||
		copy(pa, V1, av * ah);
        ERROR = norm(V1, av) * eps;
        // printf("Error: %e\n", ERROR);
		make_identity(Vmin, ah);
		if(gauss_inverse(V1, Vmin, av, ERROR) == 0) {
            // print_matrix(Vmin, av, ah, m, av);
			min   = norm(Vmin, av);
			min_i = j;
		} else c++;

		// i = j + 1, ..., k - 1 
		// A_{i, j} --> V1, V2 = (A_{i,j})^(-1)
		// if ||V2|| < min Vmin = V2, min = ||V2||
		for(i = j + 1; i * m + l < n; i++) {
			pi = A + i * n * m + j * av * m;

			copy(pi, V1, av * ah);
            ERROR = norm(V1, av) * eps;
            // printf("Error: %e\n", ERROR);
			make_identity(V2, av);
			if(gauss_inverse(V1, V2, av, ERROR) == 0) {
                
				current = norm(V2, av);
				if(fabs(current - min) < ERROR) {
					copy(V2, Vmin, av * ah);
					min   = current;
					min_i = i;
				}
			} else c++;
		}
		// if all in column noninvertable return
		if(c == k - j + (av == l)) {
			free_matrix(V1);
			free_matrix(V2);
			free_matrix(Vmin);
			return -1;
		}

		// I_min_i <--> I_j
		if(min_i != j) {
			for(i = 0; i * m < n; i++) {
				pi = A + min_i * n * m + i * m * m;
				pj = A + j * n * m + i * m * m;
				for(r = 0; r < m * m; r++) {
					current = pi[r];
					pi[r]   = pj[r];
					pj[r]   = current;			
				}
			}
		}

		//V_min * (A_{j,j}, A_{j,j+1},...,A_{j,k+1},B_{j})
		for(i = j; i * m < n; i++) {
			r = (i < k) ? m : l;
			pi = A + j * n * m + i * av * m; 
			copy(pi, V1, av * r);
			conv_basic_multiply(Vmin, av, ah, V1, av, r, V2);
			copy(V2, pi, av * r);
		}
		pb = B + j * m;
		copy(pb, V1, av * 1);
		conv_basic_multiply(Vmin, av, ah, V1, av, 1, V2);
		copy(V2, pb, av * 1);
		

		// A_{ i, c } = A_{ i, c } - A_{ i, j } x A_{ j, c }
		//      pa    =     pa     -     pi     x     pj 
		// идем по строчкам вниз
		for(i = j + 1; i * m < n; i++) {
			q = (i < k) ? m : l;
            //
            // TODO: пихать в матрицу V1, переименовать Vmin в V3
            //
			// pi = A + i * n * m + j * q * m;  
			pi = A + i * n * m + j * q * m;  
			// каждую умножаем и вычитаем с подходящим коэффицентом
			for(c = j ; c * m < n; c++) {
			// // for(c = j + 1; c * m < n; c++) {
				ah = (c < k) ? m : l;
				pa = A + i * n * m + c * q * m;
				pj = A + j * n * m + c * q * m;
				
				conv_basic_multiply(pi, q, m, pj, m, ah, V1);
				for(r = 0; r < q * ah; r++) {
					pa[r] -= V1[r];
				}
			}
			// print_matrix(A, n, n, m, r);
			pa = B + i * m;
			pj = B + j * m;
			conv_basic_multiply(pi, q, m, pj, m, 1, V2);
			for(r = 0; r < q; r++) {
				pa[r] -= V2[r];
			} 
		}

		min = 0.;
		min_i = 0;
		c = 0;
	}

	// матрица теперь верхнедиагональная
    
    // идем с последней строчки
    // copy(B, X, n);
	// for(i = k; i >= 0; i--) {
    //     av = (i < k) ? m : l;
    //     pi = X + i * m;
    //     for(j = 0; j < i; j++);
    // }
    
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

