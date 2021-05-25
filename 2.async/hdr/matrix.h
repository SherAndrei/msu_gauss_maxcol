#pragma once
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "synchronise.h"

double fabs(double);


static inline void copy(const double* source,
                        double* dest, const int v, const int h) {
    memcpy(dest, source, v * h * sizeof(double));
}

// Блок A_{i, j}
#define A(i, j) (A + (i) * n * m + (j) * av * m)
// Элемент (p, q) блока A_{i, j}
#define a(p, q) (A(i, j) + (p) * ah + (q))

// максимальная столбцовая норма большой матрицы
static inline double full_norm(struct ThreadData* p_th_data, struct SolveData sl_data) {
    int n_thr_lines = p_th_data->n_workable_lines;
    // данные потока
    int th_num = p_th_data->k;
    int n_thrd = p_th_data->p;
    // данные задачи
    const int n = sl_data.n;
    const int m = sl_data.m;
    const int k = n / m;
    const int l = n - k * m;
    double *A = sl_data.A;
    int i, j, p, q, av, ah;
    double max = 0., current = 0.;

    for (i = th_num; n_thr_lines > 0; i += n_thrd, --n_thr_lines) {
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
    p_th_data->norm = max;
    find_norm(p_th_data);
    return p_th_data->norm;
}
#undef A
#undef a

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
