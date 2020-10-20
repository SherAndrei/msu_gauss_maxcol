#pragma once

// Блок A_{i,j}
#define A(i, j) (A + (i) * n * m + (j) * av * m)
// Элемент (p, q) блока A_{i,j}
#define a(p, q) (A(i,j) + (p) * ah + (q))
// Элемент i вектора B
#define B(i) (A + n * n + (i))

double* alloc_matrix(const int length, const int width);
void free_matrix(double* matrix);

double  norm(const double* const A, const int n);
void copy(const double* source, double* dest, const int v, const int h);
void identity(double* const matr, const int dim);
void null(double* const matr, const int v, const int h);