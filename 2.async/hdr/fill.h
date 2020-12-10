#ifndef FILL_H
#define FILL_H
#include "args.h"

void formula_zero(double* const matrix, const int matrix_dim, const int block_dim,
                  const char* const filename, int* errno);

void formula_one(double* const matrix, const int matrix_dim, const int block_dim, int th_num, int n_threads);
void formula_two(double* const matrix, const int matrix_dim, const int block_dim, int th_num, int n_threads);
void formula_three(double* const matrix, const int matrix_dim, const int block_dim, int th_num, int n_threads);
void formula_four(double* const matrix, const int matrix_dim, const int block_dim, int th_num, int n_threads);
void fill_right_part(const double* const A, double* const B, const int n, const int m);

int fill(struct ThreadData* th_data, struct MainData mn_data, struct SolveData sl_data);

#endif  // FILL_H
