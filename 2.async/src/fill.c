#include "fill.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "args.h"
#include "error.h"
#include "synchronise.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// Блок A_{i, j}
#define A(i, j) (A + (i) * n * m + (j) * av * m)
// Элемент (p, q) блока A_{i, j}
#define a(p, q) (A(i, j) + (p) * ah + (q))

double fabs(double);

static void init_with_zeroes(struct SolveData s_data, struct ThreadData t_data) {
    int i;
    // данные потока
    int th_num = t_data.k;
    int n_thrd = t_data.p;
    // данные задачи
    const int n = s_data.n;
    const int m = s_data.m;
    double* const A = s_data.A;
    double* const X = s_data.X;
    double* const B = s_data.B;
    const int k = n / m;
    const int l = n - k * m;
    int av;
    for (i = th_num; i * m < n; i += n_thrd) {
        av = (i < k) ? m : l;
        memset(A + i * n * m, 0, (k * m * av + l * av) * sizeof(double));
        memset(B + i * m, 0, av * sizeof(double));
        memset(X + i * m, 0, av * sizeof(double));
    }
}

int fill(struct ThreadData* th_data, struct MainData mn_data, struct SolveData sl_data) {
    double* A = sl_data.A, *B = sl_data.B;
    int n = sl_data.n, m = sl_data.m;
    switch (mn_data.s) {
    case 0: {
        init_with_zeroes(sl_data, *th_data);
        if (th_data->k == 0) {
            formula_zero(A, n, m, mn_data.filename, &th_data->error);
        }
        collect_errors(th_data);
        if (th_data->error != 0)
            return -1;
        break;
    }
    case 1: formula_one(A, n, m, th_data->k, th_data->p);
        break;
    case 2: formula_two(A, n, m, th_data->k, th_data->p);
        break;
    case 3: formula_three(A, n, m, th_data->k, th_data->p);
        break;
    case 4: formula_four(A, n, m, th_data->k, th_data->p);
        break;
    default:
        break;
    }
    fill_right_part(A, B, n, n);
    return 0;
}

void formula_zero(double* const A, const int n, const int m,
                  const char* const filename, int* errno) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;
    int counter = 0;
    FILE* inp = fopen(filename, "r");
    if (inp == NULL) {
        *errno = error(3);
        return;
    }

    for (i = 0; i * m < n; i++) {
        av = i < k ? m : l;
        for (p = 0; p < av; p++) {
            for (j = 0; j * m < n; j++) {
            ah = j < k ? m : l;
                for (q = 0; q < ah; q++) {
                    if (fscanf(inp, "%lf", a(p, q)) == 1)
                        counter++;
                    else
                        break;
                }
            }
        }
    }
    if (counter != n * n)
        *errno = error(4);
    fclose(inp);
}

void formula_one(double* const A, const int n, const int m, int th_num, int n_thrd) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = th_num; i * m < n; i += n_thrd) {
        av = i < k ? m : l;
        for (j = 0; j * m < n; j++) {
            ah = j < k ? m : l;
            for (p = 0; p < av; p++) {
                for (q = 0; q < ah; q++) {
                    *a(p, q) = n - MAX(i * m + p + 1, j * m + q + 1) + 1;
                }
            }
        }
    }
}

void formula_two(double* const A, const int n, const int m, int th_num, int n_thrd) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = th_num; i * m < n; i += n_thrd) {
        av = i < k ? m : l;
        for (j = 0; j * m < n; j++) {
            ah = j < k ? m : l;
            for (p = 0; p < av; p++) {
                for (q = 0; q < ah; q++) {
                    *a(p, q) = MAX(i * m + p + 1, j * m + q + 1);
                }
            }
        }
    }
}

void formula_three(double* const A, const int n, const int m, int th_num, int n_thrd) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = th_num; i * m < n; i += n_thrd) {
        av = i < k ? m : l;
        for (j = 0; j * m < n; j++) {
            ah = j < k ? m : l;
            for (p = 0; p < av; p++) {
                for (q = 0; q < ah; q++) {
                    *a(p, q) = abs(i * m + p - (j * m + q));
                }
            }
        }
    }
}

void formula_four(double* const A, const int n, const int m, int th_num, int n_thrd) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = th_num; i * m < n; i += n_thrd) {
        av = i < k ? m : l;
        for (j = 0; j * m < n; j++) {
            ah = j < k ? m : l;
            for (p = 0; p < av; p++) {
                for (q = 0; q < ah; q++) {
                    *a(p, q) = fabs(1. / (i * m + p + j * m + q + 1));
                }
            }
        }
    }
}

void fill_right_part(const double* const A, double* const B, const int n, const int m) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;
    // int cnt_ev = 0, cnt_od = 0;
    int cnt = 0;

    for (i = 0; i < n; i++)
        B[i] = 0;

    // идем по строчкам
    for (i = 0; i * m < n; i++) {
        av = i < k ? m : l;
        for (p = 0; p < av; p++) {
            B[i * m + p] = 0;
            // теперь по столбикам
            // имеем счетчик, который помогает
            // нам определеить четность
            for (j = 0, cnt = 0; j * m < n; j++) {
                ah = j < k ? m : l;
                for (q = 0; q < ah; q++, cnt++) {
                    if (cnt % 2 == 0)
                        B[i * m + p] += *a(p, q);
                }
            }
        }
    }
}
