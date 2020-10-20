#pragma once

void formula_zero (double* const matrix, const int matrix_dim, const int block_dim, 
					const char* const filename, int* errno);
void formula_one  (double* const matrix, const int matrix_dim, const int block_dim);
void formula_two  (double* const matrix, const int matrix_dim, const int block_dim);
void formula_three(double* const matrix, const int matrix_dim, const int block_dim);
void formula_four (double* const matrix, const int matrix_dim, const int block_dim);

void fill(double* const A, const int n, const int m, const int s, const char* const filename, int* const errno);
void fill_right_part(const double* const A, double* const B, const int n, const int m);