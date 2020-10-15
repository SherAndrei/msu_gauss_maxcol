#pragma once

#define Matrix(i, j) matrix[(i)*dim + (j)]

double* alloc_matrix(const int length, const int width);