#include "fill.h"
#include "formula.h"
#include "error.h"
#include "matrix.h"
#include <stdio.h>

//заполнить матрицу с помощью формулы номер formula_name
void fill(double* matrix, const int matrix_dim, const int block_dim, const int formula_name, const char* const filename, int* errno)
{
    switch (formula_name)
    {
	case 0: formula_zero(matrix, matrix_dim, block_dim, filename, errno);
		break;
    case 1: formula_one(matrix, matrix_dim, block_dim);
        break;
    case 2: formula_two(matrix, matrix_dim, block_dim);
        break;
    case 3: formula_three(matrix, matrix_dim, block_dim);
        break;
    case 4: formula_four(matrix, matrix_dim, block_dim);
        break;
    default:
        break;
    }
}

//заполнить правую часть с помощью формулы из условия
void fill_right_part(double* A, double* B, const int n, const int m)
{
	int i, j, p, q, last_i, last_j;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;
	// int cnt_ev = 0, cnt_od = 0;
	int cnt = 0;

	for(i = 0; i < n; i++) 
		B[i] = 0;
	//	
	// TODO: upgrage for cash
	//

	//идем по строчкам
	for(i = 0; i * m < n; i++) {
		last_i = i < k ? m : l;
		for(p = 0; p < last_i; p++) {
			B[i * m + p] = 0;
			// теперь по столбикам
			// имеем счетчик, который помогает
			// нам определеить четность
			for(j = 0, cnt = 0; j * m < n; j++) {
				last_j = j < k ? m : l;
				for(q = 0; q < last_j; q++, cnt++) {
					if(cnt % 2 == 0)
						B[i * m + p] += *a(p, q); 
				}
			}
		}
	}
}


