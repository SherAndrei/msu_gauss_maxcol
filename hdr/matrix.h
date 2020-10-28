#pragma once

double* alloc_matrix(const int length, const int width);
void free_matrix(double* matrix);

double  norm(const double* const A, const int n);
void copy(const double* source, double* dest, const int v, const int h);
void identity(double* const matr, const int dim);
void null(double* const matr, const int v, const int h);