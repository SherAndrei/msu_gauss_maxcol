#pragma once
#include <stdlib.h>
#include <string.h>

inline double* alloc_matrix(const int size) {
    return (double*) malloc (size * sizeof(double));
}
inline void free_matrix(double* matrix) {
    free(matrix);
}
inline void copy(const double* source, double* dest, const int v, const int h) {
    memcpy(dest, source, v * h * sizeof(double));
}

double  norm(const double* const A, const int n);
void identity(double* const matr, const int dim);
void null(double* const matr, const int v, const int h);
