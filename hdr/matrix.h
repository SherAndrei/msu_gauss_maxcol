#pragma once

// Блок A_{i,j}
#define A(i, j) (A + (i) * n * m + (j) * last_i * m)
// Элемент (p, q) блока A_{i,j}
#define a(p, q) (A(i,j) + (p) * last_j + (q))
// Элемент i вектора B
#define B(i) (A + n * n + (i))


double* alloc_matrix(const int length, const int width);