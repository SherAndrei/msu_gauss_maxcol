#include <math.h>
#include "gauss_inverse.h"
#define LOG 0
#if LOG
#include <stdio.h>
#endif

//Поменять местами элемент lhs и rhs
static void swap(double* lhs, double* rhs) 
{
    double temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

#define eps 1e-16

// сделать единичной matr
void make_identity(double* matr, const int dim)
{
	int i, j;
	for(i = 0; i < dim; i++) 
		for(j = 0; j < dim; j++)
			matr[i*dim + j] = ( i == j );
}

#define A(i,j) A[(i) * n + (j)]
#define E(i,j) A_inversed[(i) * n + (j)]

int gauss_inverse(double * A, double* A_inversed, const int n, double ERROR)
{
	// итераторы
	int i, j, k; 
	// максимальный элемент по столбцу
	double max = 0.;
	// строчка и столбец с максимальным элементом
	int max_i = 0.;
	// коэффициент 
	double c = 0.;
#if LOG
	printf("%e\n", ERROR);
#endif
	//идем по столбцам
	for(j = 0; j < n; j++) {

		// назначаем элемент на диагонали главным
		// и соответстенную строчку
		max = fabs(A(j,j));
		max_i = j;

		// ищем максимальный элемент в столбце
		// и строчку с ним
		for(i = j; i < n; i++) {
			if(fabs(A(i,j)) > max) {
				max_i = i;
				max   = A(i,j);
			}
		}

		if(fabs(max) <= ERROR) {
            return -1;
		}

		if(max_i != j) {
			// меняем строчки местами
			for(i = 0; i < j; i++)
				swap(&(E(j,i)), &(E(max_i, i)));			
			for(i = j; i < n; i++) {
				swap(&(A(j,i)), &(A(max_i, i)));			
				swap(&(E(j,i)), &(E(max_i, i)));			
			}
		}
		//Так как элемент a_jj на диагонали != 0, то
        //разделим j уравнение на него  
        c = A(j,j);
		if(fabs(c - 1) > ERROR) {
			for(k = 0; k < j; k++)
				E(j, k) /= c;
        	for(k = j; k < n; k++) {
	    	    A(j, k) /= c;
	    	    E(j, k) /= c;
        	}
			ERROR /= fabs(c);
		}

		// a_jj = 1
        //вычитаем из всех строчек строчку с максимальным элементом,
        // умноженную на соответствующий коэфф c, чтобы получить нули в столбце 
		for(i = j + 1; i < n; i++) {
			c = A(i, j);
			if(fabs(c) > ERROR) {
				for(k = 0; k < j; k++) {
					E(i, k) -= c * E(j,k);
				}
				for(k = j; k < n; k++) {
					A(i, k) -= c * A(j,k);
					E(i, k) -= c * E(j,k);
				}
			}
			// ERROR += fabs(c)*ERROR;
		}
	}
	// матрица теперь верхнедиагональная
	//обратный ход метода Гаусса
	//идем по столбцам
	for(j = n - 1; j >= 0; j--) {
		for(i = 0; i < j; i++) {
			c = A(i,j);
			if(fabs(c) > ERROR) {
				for(k = 0; k < n; k++)
					E(i,k) -= c * E(j,k);
				A(i,j) = 0; 
			}
		}
	}
	return 0;
}
