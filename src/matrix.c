#include "matrix.h"
#include <stdlib.h>
#include <string.h>

double fabs(double);

inline double* alloc_matrix(const int size) {
    return (double*) malloc (size * sizeof(double));
}

inline void free_matrix(double* matrix) {
    free(matrix);
}

// максимальная столбцовая норма матрицы
double  norm(const double* const A, const int n) {
    int i, j;
    double max = 0., current = 0.;
    for (i = 0; i < n; i++) {
        current = 0.;
        for (j = 0; j < n; j++) {
            current += fabs(A[j * n + i]);
        }
        if (current > max)
            max = current;
    }
    return max;
}

inline void copy(const double* source, double* dest, const int v, const int h) {
    memcpy(dest, source, v * h * sizeof(double));
}

void identity(double* const matr, const int dim) {
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            matr[i*dim + j] = (i == j);
}

void null(double* const matr, const int v, const int h) {
    int i;
    for (i = 0; i < v * h; i++)
        matr[i] = 0.;
}

