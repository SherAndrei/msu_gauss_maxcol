#include "formula.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

void formula_zero(double* matrix, const int dim, 
				const char* const filename, int* errno)
{
	FILE* inp = fopen(filename, "r");
    if(inp == NULL) {
        *errno = 6;
		return;
	}

	(void) matrix;
    int counter = 0;
    // for(int i = 0; i < dim * dim; ++i)
    //     if(fscanf(inp, "%lf", &(matrix[i])) == 1)
    //         counter++;
    //     else break;
            
    if(counter != dim * dim)
        *errno = 7;

    fclose(inp);
}

//Формула 1 из условия
void formula_one(double* matrix, const int dim)
{
    for(int i = 0; i < dim; ++i)
        for(int j = 0; j < dim; ++j)
            Matrix(i, j) = dim - MAX(i + 1, j + 1) + 1;
}
//Формула 2 из условия
void formula_two(double* matrix, const int dim)
{
    for(int i = 0; i < dim; ++i)
        for(int j = 0; j < dim; ++j)
            Matrix(i, j) = MAX(i + 1, j + 1);
}
//Формула 3 из условия
void formula_three(double* matrix, const int dim)
{
    for(int i = 0; i < dim; ++i)
        for(int j = 0; j < dim; ++j)
            Matrix(i, j) = abs(i - j);
}
//Формула 4 из условия
void formula_four(double* matrix, const int dim)
{
    for(int i = 0; i < dim; ++i)
        for(int j = 0; j < dim; ++j)
            Matrix(i, j) = fabs(1.  / (i + j + 1));
}