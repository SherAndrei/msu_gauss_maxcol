#include "formula.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

void formula_zero(double* A, const int n, const int m, 
				const char* const filename, int* errno)
{
	FILE* inp = fopen(filename, "r");
    if(inp == NULL) {
        *errno = 6;
		return;
	}

	(void) A;
	(void) n;
	(void) m;

	// int counter = 0;
    // // for(int i = 0; i < dim * dim; ++i)
    // //     if(fscanf(inp, "%lf", &(matrix[i])) == 1)
    // //         counter++;
    // //     else break;
            
    // if(counter != dim * dim)
    //     *errno = 7;

    fclose(inp);
}

//Формула 1 из условия
void formula_one(double* A, const int n, const int m)
{
	// double* block;
	int i, j, p, q, last;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;

	for(i = 0; i * m < n; i++) {
		last = i < k ? m : l;
		for(j = 0; j * m < n; j++) {
			for(p = 0; p < last; p++) {
				for(q = 0; q < last; q++) {
					A[i * n * m + j * last * m + p * last + q] = n - MAX(p + 1, q + 1) + 1;
				}
			}
		}
	}
    // for(int i = 0; i < dim; ++i)
    //     for(int j = 0; j < dim; ++j)
    //         Matrix(i, j) = dim - MAX(i + 1, j + 1) + 1;
}
//Формула 2 из условия
void formula_two(double* A, const int n, const int m)
{
	(void) A;
	(void) n;
	(void) m;
    // for(int i = 0; i < dim; ++i)
    //     for(int j = 0; j < dim; ++j)
    //         Matrix(i, j) = MAX(i + 1, j + 1);
}
//Формула 3 из условия
void formula_three(double* A, const int n, const int m)
{
	(void) A;
	(void) n;
	(void) m;
    // for(int i = 0; i < dim; ++i)
    //     for(int j = 0; j < dim; ++j)
    //         Matrix(i, j) = abs(i - j);
}
//Формула 4 из условия
void formula_four(double* A, const int n, const int m)
{
	(void) A;
	(void) n;
	(void) m;
    // for(int i = 0; i < dim; ++i)
    //     for(int j = 0; j < dim; ++j)
    //         Matrix(i, j) = fabs(1.  / (i + j + 1));
}