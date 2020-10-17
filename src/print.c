#include "print.h"
#include "matrix.h"
#include "solve.h"
#include <stdio.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

// Печать матрицы соответственно условию
void print_matrix(const double* A, const int n, const int m, const int print_value)
{
	(void) print_value;
	int i, j, p, q, last_i, last_j;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;

	for(i = 0; i * m < n; i++) {
		last_i = i < k ? m : l;
		for(p = 0; p < last_i; p++) {
			for(j = 0; j * m < n; j++) {
				last_j = j < k ? m : l;
				for(q = 0; q < last_j; q++) {
					printf("%10.3e ", *a(p, q));
					// *a(p, q) = n - MAX( j * m + q + 1, i * m + p + 1) + 1;
				}
			}
			putchar('\n');
		}
	}
    // for(int i = 0; i < MIN(Nrows, print_value); ++i)
    // {
    //     for(int j = 0; j < MIN(Ncolumns, print_value); ++j)
    //         printf("%10.3e ", matrix[i*Nrows + j]);
        
    //     printf("\n");
    // }
    // printf("\n");
}

//Печать матрицы, ответа и времени
void print_all(double* matrix, const double* answer, const int dim, const int print_value, float time)
{
        printf("==================================\n\n");
        print_matrix(matrix, dim, dim, print_value);
        print_matrix(answer, 1, dim, print_value);
        printf("Time of solving : %6.3f sec\n", time);
}   

//Печать нормы невязки
void print_residual(const double* matrix, const double* r_part, const int dim, const double* answer)
{ 
    printf("Residual norm   : %10.3e\n", residual(dim, matrix, r_part, answer)); 
}
//Печать нормы разности между ответами
void print_difference(const int dim, double* answer)
{ 
    printf("Differense norm : %10.3e\n", difference(dim, answer)); 
}