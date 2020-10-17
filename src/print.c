#include "print.h"
#include "matrix.h"
#include "solve.h"
#include <stdio.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

// Печать матрицы соответственно условию
void print_matrix(const double* A, const int n, const int hor, const int m, const int r)
{
	(void) hor;
	// количество печатаемых блоков
	int n1 = MIN(n, r);
	// размер очередного блока
	int mv, mh;
	// количество печатаемых данных в данном блоке
	int prn_val_h = r;
	int prn_val_v = r;
	//для циклов
	int i, j, p, q;
	//количество в последнем блоке
	int last_i, last_j;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;

	for(i = 0; i * m < n1; i++) {
		last_i = i < k ? m : l;
		mv = MIN(last_i, prn_val_v);
		for(p = 0; p < mv; p++) {
			prn_val_h = r;
			for(j = 0; j * m < n1; j++) {
				last_j = j < k ? m : l;
				mh = MIN(last_j, prn_val_h);
				for(q = 0; q < mh; q++) {
					printf(" %10.3e", *a(p, q));
				}
				prn_val_h -= last_j;
			}
			putchar('\n');
		}
		prn_val_v -= last_i;
	}
}

// //Печать матрицы, ответа и времени
// void print_all(double* matrix, const double* answer, const int dim, const int r, float time)
// {
//         printf("==================================\n\n");
//         print_matrix(matrix, dim, dim, r);
//         print_matrix(answer, 1, dim, r);
//         printf("Time of solving : %6.3f sec\n", time);
// }   

// //Печать нормы невязки
// void print_residual(const double* matrix, const double* r_part, const int dim, const double* answer)
// { 
//     printf("Residual norm   : %10.3e\n", residual(dim, matrix, r_part, answer)); 
// }
// //Печать нормы разности между ответами
// void print_difference(const int dim, double* answer)
// { 
//     printf("Differense norm : %10.3e\n", difference(dim, answer)); 
// }