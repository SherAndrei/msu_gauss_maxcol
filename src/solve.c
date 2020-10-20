#include "matrix.h"
#include "error.h"
#include "print.h"
#include "multiply.h"
#include "gauss_inverse.h"

double fabs(double);
double sqrt(double);

#ifndef NULL
#define NULL ((void *) 0)
#define eps (1e-16)

//Найти корни и записать в answer
int solve(const int n, const int m, double* A, double* B, double* X)
{
	// итераторы
	int i = 0, j = 0, r = 0, q = 0;
	// вспомогательные матрицы
	double *V1, *V2, *V3;
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
        ERROR = norm(V1, av) * eps;
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
            ERROR = norm(V1, av) * eps;
			identity(V2, av);
			if(gauss_inverse(V1, V2, av, ERROR) == 0) {
                
				current = norm(V2, av);
				if(fabs(current - min) < ERROR) {
					copy(V2, V3, av, ah);
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

		//A_{j,j} = E, V_3 * (A_{j,j+1},...,A_{j,k+1},B_{j})
        identity(pa, av);
		for(i = j + 1; i * m < n; i++) {
			r = (i < k) ? m : l;
			pi = A + j * n * m + i * av * m; 
			copy(pi, V1, av, r);
			conv_basic_multiply(V3, av, ah, V1, av, r, V2);
			copy(V2, pi, av, r);
		}
		pb = B + j * m;
		copy(pb, V1, av, 1);
		conv_basic_multiply(V3, av, ah, V1, av, 1, V2);
		copy(V2, pb, av, 1);
		

		// A_{ i, c } = A_{ i, c } - A_{ i, j } x A_{ j, c }
		//      pa    =     pa     -     pi     x     pj 
		// идем по строчкам вниз
		for(i = j + 1; i * m < n; i++) {
			q = (i < k) ? m : l;
			pi = A + i * n * m + j * q * m;  
			// каждую умножаем и вычитаем с подходящим коэффицентом
			for(c = j + 1; c * m < n; c++) {
				ah = (c < k) ? m : l;
				pa = A + i * n * m + c * q * m;
				pj = A + j * n * m + c * q * m;
				
				conv_basic_multiply(pi, q, m, pj, m, ah, V1);
				for(r = 0; r < q * ah; r++) {
					pa[r] -= V1[r];
				}
			}
			pa = B + i * m;
			pj = B + j * m;
			conv_basic_multiply(pi, q, m, pj, m, 1, V2);
			for(r = 0; r < q; r++) {
				pa[r] -= V2[r];
			} 
            null(pi, q, m);
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
        // pi = X + i * m;
        // pj = B + i * m;
        copy(B + i * m, X + i * m, m, 1);
        // сумма наших матриц
        // null(V1, m, 1);
        for(j = i + 1; j * m < n; j++) {
            ah = (j < k) ? m : l;
            // A_{i,j}
            // уже не работаем с матрицами l x m
            pi = A + i * m * n + j * m * m; 
            // X_{j}
            pj = X + j * m;

            conv_basic_multiply(pi, m, ah, pj, ah, 1, V2);
            for(q = 0; q < m * 1; q++)
                (X + i * m)[q] -= V2[q];
            null(pi, m, ah); 
        }
    }

	free_matrix(V1);
	free_matrix(V2);
	free_matrix(V3);
	return 0;
}


//Норма невязки
double residual(const double* A, const double* B, const double* X, const int n, const int m)
{
(void) A;
(void) B;
(void) X;
(void) n;
(void) m;
	return 0.;
}

//Норма разности с ответом
double difference(const double* const answer, const int dim)
{
    double result = 0;
    int i;
    for(i = 0; i < dim; i += 2) {
        result += (answer[i] - 1) * (answer[i] - 1);
    }

    return sqrt(result);
}

#endif // NULL
#undef eps