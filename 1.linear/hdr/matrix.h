#pragma once
#include <stdlib.h>
#include <string.h>

double fabs(double);

static inline void copy(const double* source,
                        double* dest, const int v, const int h) {
    memcpy(dest, source, v * h * sizeof(double));
}

// Блок A_{i, j}
#define A(i, j) (A + (i) * n * m + (j) * av * m)
// Элемент (p, q) блока A_{i, j}
#define a(p, q) (A(i, j) + (p) * ah + (q))

static inline double full_norm(const double* const A, const int n, const int m) {
    const int k = n / m;
    const int l = n - k * m;
    int i, j, p, q, av, ah;
    double max = 0., current = 0.;

    for (i = 0; i * m < n; i++) {
        av = i < k ? m : l;
        for (p = 0; p < av; p++) {
            current = 0.;
            for (j = 0; j * m < n; j++) {
                ah = j < k ? m : l;
                for (q = 0; q < ah; q++) {
                    current += fabs(*a(p, q));
                }
            }
            if (current > max)
                max = current;
        }
    }
    return max;
}
#undef A
#undef a

// нельзя считать для всей матрицы! другое хранение памяти
// максимальная столбцовая норма матрицы
static inline double  norm(const double* const A, const int n) {
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


static inline void identity(double* const matr, const int dim) {
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            matr[i*dim + j] = (i == j);
}

static inline void null(double* const matr, const int v, const int h) {
    int i;
    for (i = 0; i < v * h; i++)
        matr[i] = 0.;
}

