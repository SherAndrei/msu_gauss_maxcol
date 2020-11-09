#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fill.h"
#include "error.h"
#include "matrix.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// Блок A_{i, j}
#define A(i, j) (A + (i) * n * m + (j) * av * m)
// Элемент (p, q) блока A_{i, j}
#define a(p, q) (A(i, j) + (p) * ah + (q))

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
        *errno = 6;
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
        *errno = 7;
    fclose(inp);
}

void formula_one(double* const A, const int n, const int m) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = 0; i * m < n; i++) {
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
void formula_two(double* const A, const int n, const int m) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = 0; i * m < n; i++) {
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
void formula_three(double* const A, const int n, const int m) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = 0; i * m < n; i++) {
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
void formula_four(double* const A, const int n, const int m) {
    int i, j, p, q, av, ah;
    // количество блоков размера n
    const int k = n / m;
    // длина/высота остаточного блока
    const int l = n - k * m;

    for (i = 0; i * m < n; i++) {
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

void fill(double* const A, const int n, const int m, const int s,
          const char* const filename, int* const errno) {
    switch (s) {
    case 0: formula_zero(A, n, m, filename, errno);
        break;
    case 1: formula_one(A, n, m);
        break;
    case 2: formula_two(A, n, m);
        break;
    case 3: formula_three(A, n, m);
        break;
    case 4: formula_four(A, n, m);
        break;
    default:
        break;
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
