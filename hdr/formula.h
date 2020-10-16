#pragma once

void formula_zero (double* matrix, const int matrix_dim, int block_dim, 
					const char* const filename, int* errno);
void formula_one  (double* matrix, const int matrix_dim, const int block_dim);
void formula_two  (double* matrix, const int matrix_dim, const int block_dim);
void formula_three(double* matrix, const int matrix_dim, const int block_dim);
void formula_four (double* matrix, const int matrix_dim, const int block_dim);