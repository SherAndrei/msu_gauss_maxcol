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
void fill_right_part(const double* matrix, double* right_part, const int dim)
{
    for(int i = 0; i < dim; i++) {
        for(int k = 0; k < (dim + 1)/2; k++) {
            //так как нумерация с нуля ~~~\/ не прибавляем 1
            right_part[i] += Matrix(i, 2*k);
        }
    }
}