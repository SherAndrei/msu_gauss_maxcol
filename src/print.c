#include "print.h"
#include "matrix.h"
#include "solve.h"
#include <stdio.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

// Печать матрицы соответственно условию
// A size : h x w (rows x cols)
// A_{i,j} size: av * ah
void print_matrix(const double* A, const int v, const int h, const int m, const int r)
{
	// количество печатаемых элементов в ширину
	int nv = MIN(v, r);
	// количество печатаемых элементов в высоту
	int nh = MIN(h, r);
	// размер очередного блока
	int mv, mh;
	// количество печатаемых данных в данном блоке
	int prn_val_h = r;
	int prn_val_v = r;
	//для циклов
	int i, j, p, q;
	//количество в последнем блоке
	int av, ah;
	// количество блоков размера m в высоту
	int kh = h / m;
	// количество блоков размера m в ширину
	int kv = v / m;
	// длина/высота остаточного блока
	int lh = nh - kh * m;
	// длина/высота остаточного блока
	int lv = nv - kv * m;

	for(i = 0; i * m < nh; i++) {
		av = i < kh ? m : lh;
		mv = MIN(av, prn_val_v);
		for(p = 0; p < mv; p++) {
			prn_val_h = r;
			for(j = 0; j * m < nv; j++) {
				ah = j < kv ? m : lv;
				mh = MIN(ah, prn_val_h);
				for(q = 0; q < mh; q++) {
					printf(" %10.3e", *(A + (i) * h * m + (j) * av * m  + (p) * ah + (q)));
				}
				prn_val_h -= ah;
			}
			putchar('\n');
		}
		prn_val_v -= av;
	}
	putchar('\n');
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