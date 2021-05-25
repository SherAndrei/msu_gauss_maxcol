#ifndef SOLVE_H
#define SOLVE_H
#include <stdlib.h>
#include "matrix.h"
#include "thread_func.h"
#include "gauss_inverse.h"
#include "operations.h"

#define eps (1e-15)

static inline int solve(args_t* a, double* V1, double* V2, double* V3,
                        double* Buffer) {
    // итераторы
    int i, j, r, q;
    // данные потока
    int n_thr_lines = a->thrd_data.n_workable_lines;
    int th_num = a->thrd_data.k;
    int n_thrd = a->thrd_data.p;
    // данные задачи
    const int n = a->slve_data.n;
    const int m = a->slve_data.m;
    const int k = n / m;
    const int l = n - k * m;
    const int i1 = th_num * (k + (l != 0)) / n_thrd;
    const int i2 = (th_num + 1) * (k + (l != 0)) / n_thrd;
    // длина/ширина текущего блока
    int av, ah;
    double* const A = a->slve_data.A;
    // double* const B = a->slve_data.B;
    double *pa, *pi, *pj;
    args_t* iter = 0;
    // счетчик необратимых матриц в столбце
    int c = 0;
    // текущая норма
    double current = 0.;
    double min = 0.;
    int min_i = 0;
    double ERROR = full_norm(&(a->thrd_data), a->slve_data) * eps;

    for (j = 0; j * m < n; j++) {
        av = ah = j < k ? m : l;
        pa = A + (th_num + j) * n * m + j * av * m;

        // copy && identity
        if (n_thr_lines > 0) {
            for (r = 0; r < av; r++) {
                for (q = 0; q < ah; q++) {
                    V1[r * ah + q] = pa[r * ah + q];
                    V3[r * ah + q] = (r == q);
                }
            }
            if (gauss_inverse(V1, V3, av, ERROR) == 0) {
                min = norm(V3, av);
                min_i = (th_num + j);
            } else {
                min = 0.;
                c++;
            }
            --n_thr_lines;
        }

        for (i = th_num + n_thrd;
             i * m + l < n && n_thr_lines > 0;
             i += n_thrd, --n_thr_lines) {
            pi = A + i * n * m + j * av * m;
            // copying pi --> V1 && identity(V2)
            for (r = 0; r < av; r++) {
                for (q = 0; q < ah; q++) {
                    V1[r * ah + q] = pi[r * ah + q];
                    V2[r * ah + q] = (r == q);
                }
            }
            if (gauss_inverse(V1, V2, av, ERROR) == 0) {
                current = norm(V2, av);
                if (current < min) {
                    pj = V2;
                    V2 = V3;
                    V3 = pj;
                    min   = current;
                    min_i = i;
                }
            } else {
                c++;
            }
        }
        a->thrd_data.result = c;

        find_max(a, V3, min, m, &min_i);

        if (a->thrd_data.result == k - j + (av == l)) {
            return -1;
        }

        if ((j - th_num) % n_thrd == 0) {
            for (i = 0; i * m < n; i++) {
                q  = (i < k) ? m : l;
                pi = A + min_i * n * m + i * m * m;
                pj = A + j * n * m + i * m * m;

                // swap pi and pj
                for (r = 0; r < m; r++) {
                    for (c = 0; c < q; c++) {
                        current       = pi[r * q + c];
                        pi[r * q + c] = pj[r * q + c];
                        pj[r * q + c] = current;
                    }
                }
            }
            // pi = B + min_i * m;
            // pj = B + j * m;
            // // swap pi and pj
            // for (r = 0; r < m; r++) {
            //     current = pi[r];
            //     pi[r]   = pj[r];
            //     pj[r]   = current;
            // }
        }

        /*
        Загрузим min_i строчку в по частям каждому потоку в буфер
        умножим каждым потоком свою часть на обратную
        объединим буферы
        поставим буфер обладателя jой строчки на jую строчку
        остальные будут в это время начинать считать свои матрицы
        */

        n_thr_lines = a->thrd_data.n_workable_lines;
        if (n_thr_lines > 0) {
            memcpy(Buffer + i1 * m * m,
                   A + min_i * m * n + i1 * m * av,
                   (i2 - i1) * m * av * sizeof(double));
            for (i = i1; i != i2; i++) {
                r = (i < k) ? m : l;
                pi = Buffer + i * m * av;
                copy(pi, V1, av, r);
                multiply(V3, av, ah, V1, av, r, V2);
                copy(V2, pi, av, r);
            }
            // TODO B

            for (iter = a->begin; iter != a->end; iter++) {
                av = (iter - a->begin) * (i2 - i1) * m * m;
                ah = (iter != a->end - 1) ? m : l;
                if (iter != a) {
                    memcpy(Buffer + av,
                        iter->assist.Buffer + av,
                        (i2 - i1) * m * av * sizeof(double));
                }
            }
        }

        barrier(&a->thrd_data);

        for (i = j + th_num + 1;
             i * m < n && n_thr_lines > 0;
             i += n_thrd, --n_thr_lines) {
            q = (i < k) ? m : l;
            pi = A + i * n * m + j * q * m;
            copy(pi, V1, q, m);
            for (c = j + 1; c * m < n; c++) {
                ah = (c < k) ? m : l;
                pa = A + i * n * m + c * q  * m;
                pj = Buffer + c * av * m;

                copy(pj, V2, m, ah);
                multiply_and_extract(V1, q, m, V2, m, ah, pa);
            }
            null(pi, q, m);
            // TODO B
            // pa = B + i * m;
            // pj = B + j * m;
            // copy(pj, V2, m, 1);
            // multiply_and_extract(V1, q, m, V2, m, 1, pa);
        }


        a->thrd_data.result = 0;
        n_thr_lines = a->thrd_data.n_workable_lines;
        barrier(&a->thrd_data);
    }

    return 0;
}

#endif  // SOLVE_H
