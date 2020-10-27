#include "solve.h"
#include "matrix.h"
#include "error.h"
#include "multiply.h"
#include "extract.h"
#include "gauss_inverse.h"
#include "print.h"

double fabs(double);

#define eps (1e-15)

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef DEBUG
#include <stdio.h>
#define PRINTV 15
#endif

//Найти корни и записать в answer
int solve(const int n, const int m, double* A, double* B, double* X)
{
	// итераторы
	int i = 0, j = 0, r = 0, q = 0;
	// вспомогателusьные матрицы
	double *V1, *V2, *V3;
	// указатель на текущий блок
	double *pa ;
	// указатель на очередной блок в столбце/в строчке
	double *pi, *pj;
	// размер текущего блока av * ah
	int av = 0, ah = 0;
	// количество блоков
	const int k = n / m;
	// остаток
	const int l = n - k * m;
	// погрешность
    double ERROR = (norm(A, n) * eps);
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
	if((V3 = alloc_matrix(m, m)) == NULL) {
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
		copy(pa, V1, av, ah);
		identity(V3, ah);
		if(gauss_inverse(V1, V3, av, ERROR) == 0) {
			min   = norm(V3, av);
			min_i = j;
		} else c++;

		// i = j + 1, ..., k - 1 
		// A_{i, j} --> V1, V2 = (A_{i,j})^(-1)
		// if ||V2|| < min V3 = V2, min = ||V2||
		for(i = j + 1; i * m + l < n; i++) {
			pi = A + i * n * m + j * av * m;

			copy(pi, V1, av, ah);
			identity(V2, av);
			if(gauss_inverse(V1, V2, av, ERROR) == 0) {
				current = norm(V2, av);
				if(fabs(current - min) > ERROR) {
                    pj = V2;
                    V2 = V3;
                    V3 = pj;
					min   = current;
					min_i = i;
				}
			} else c++;
		}
		// if all in column noninvertable return
		if(c == k - j + (av == l)) {
			free_matrix(V1);
			free_matrix(V2);
			free_matrix(V3);
			return -1;
		}
#ifdef DEBUG
        printf("Initial matrix:\n");
        print_matrix(A, n, n, m, PRINTV);    
        print_matrix(B, n, 1, m, PRINTV);    
#endif
		// I_min_i <--> I_j
		if(min_i != j) {
			for(i = 0; i * m < n; i++) {
                q  = (i < k) ? m : l;  
				pi = A + min_i * n * m + i * m * m;
				pj = A + j * n * m + i * m * m;

                copy(pi, V1, m, q);
                copy(pj, pi, m, q);
                copy(V1, pj, m, q);
			}
            pi = B + min_i * m;
            pj = B + j * m;
            copy(pi, V1, m, 1);
            copy(pj, pi, m, 1);
            copy(V1, pj, m, 1);
#ifdef DEBUG
        printf("Swap %d and %d:\n", min_i, j);
        print_matrix(A, n, n, m, PRINTV);    
        print_matrix(B, n, 1, m, PRINTV);    
#endif
		}

		//A_{j,j} = E, V_3 * (A_{j,j+1},...,A_{j,k+1},B_{j})
        identity(pa, av);
#ifdef DEBUG
        print_matrix(A, n, n, m, PRINTV);    
        print_matrix(B, n, 1, m, PRINTV);    
#endif
		for(i = j + 1; i * m < n; i++) {
			r = (i < k) ? m : l;
			pi = A + j * n * m + i * av * m; 
			copy(pi, V1, av, r);
			conv_basic_multiply(V3, av, ah, V1, av, r, V2);
			copy(V2, pi, av, r);
		}
		pi = B + j * m;
		copy(pi, V1, av, 1);
		conv_basic_multiply(V3, av, ah, V1, av, 1, V2);
		copy(V2, pi, av, 1);
#ifdef DEBUG
        printf("Multiplying:\n");
        print_matrix(A, n, n, m, PRINTV);    
        print_matrix(B, n, 1, m, PRINTV);    
#endif
		// A_{ i, c } = A_{ i, c } - A_{ i, j } x A_{ j, c }
		//      pa    =     pa     -     pi     x     pj 
		// идем по строчкам вниз
		for(i = j + 1; i * m < n; i++) {
			q = (i < k) ? m : l;
			pi = A + i * n * m + j * q * m;  
            copy(pi, V1, q, m);
			// каждую умножаем и вычитаем с подходящим коэффицентом
			for(c = j + 1; c * m < n; c++) {
				ah = (c < k) ? m : l;
    			pa = A + i * n * m + c * q  * m;
    			pj = A + j * n * m + c * av * m;
				
    			conv_basic_multiply(V1, q, m, pj, m, ah, V3);
                extract(pa, V3, q, ah);
			}
			pa = B + i * m;
			pj = B + j * m;
    		conv_basic_multiply(V1, q, m, pj, m, 1, V3);
    		extract(pa, V3, 1, q);
            null(pi, q, m);
#ifdef DEBUG
        printf("formula:\n");
        print_matrix(A, n, n, m, PRINTV);    
        print_matrix(B, n, 1, m, PRINTV);    
#endif
		}
        min = 0.;
		min_i = 0;
		c = 0;
	}
    // матрица теперь верхнедиагональная
    
    // последние X_{l} нам уже известны
    copy(B + k * m, X + k * m, l, 1);
    // идем с последней строчки
    for(i = k - 1; i >= 0; i--) {
        copy(B + i * m, X + i * m, m, 1);
        // сумма наших матриц
        for(j = i + 1; j * m < n; j++) {
            ah = (j < k) ? m : l;
            // A_{i,j}
            // уже не работаем с матрицами l x m
            pi = A + i * m * n + j * m * m; 
            // X_{j}
            pj = X + j * m;

            conv_basic_multiply(pi, m, ah, pj, ah, 1, V2);
            extract((X + i * m), V2, 1, m);
            null(pi, m, ah); 
        }
    }
#ifdef DEBUG
    print_matrix(A, n, n, m, PRINTV);    
    print_matrix(B, n, 1, m, PRINTV);  
    print_matrix(X, n, 1, m, PRINTV);  
#endif
    free_matrix(V1);
	free_matrix(V2);
	free_matrix(V3);
	return 0;
}


//Норма невязки
double residual(double* A, double* B, double* X, const int n, const int m)
{
    int i, j, q, r;
    int av, ah;
    double *pa, *pi, *pj;
    // количество блоков
	const int k = n / m;
	// остаток
	const int l = n - k * m;
    double norm = 0.;
    double sum = 0.;

    // Норма B
    for(i = 0; i < n; i++) {
        norm += fabs(B[i]);
    }
    // идем по столбцам
    for(j = 0; j * m < n; j++) {
        ah = (j < k) ? m : l;
        pj = X + j * m;
        // по каждой строчке
        for(i = 0; i * m < n; i++) {
            pi = B + i * m;
            av = (i < k) ? m : l;
            pa = A + i * m * n + j * av * m;
            // считаем B_{i} = B_{i} - \sum A_{i,j} X_{j} 
            for(q = 0; q < av; q++) {
                sum = 0.;
                for(r = 0; r < ah; r++) {
                    sum += pa[q * ah + r] * pj[r];
                }
                pi[q] -= sum;
            }
        }
    }
    sum = 0.;
    // Норма невязки
    for(i = 0; i < n; i++) {
        sum += fabs(B[i] / norm);
    }

	return sum;
}

//Векторная норма разности с ответом
double difference(const double* const answer, const int dim)
{
    double result = 0;
    int i;
    for(i = 0; i < dim; i++) {
        result += fabs(answer[i] - !(i & 1));
    }

    return result;
}

#undef eps