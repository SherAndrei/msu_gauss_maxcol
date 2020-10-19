#include "matrix.h"
#include <stdlib.h>

//аллоцировать память под матрицу length*width
double* alloc_matrix(const int length, const int height) {
    return (double*) malloc (length*height*sizeof(double));
}

void free_matrix(double* matrix) {
	free(matrix);
}
