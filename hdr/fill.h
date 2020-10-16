#pragma once

void fill(double* matrix, const int matrix_dim, const int block_dim, const int formula_name, const char* const filename, int* errno);
void fill_right_part(const double* matrix, double* right_part, const int dim);