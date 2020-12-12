#ifndef SOLVE_H
#define SOLVE_H
#include <stdlib.h>
#include "matrix.h"
#include "thread_func.h"
#include "gauss_inverse.h"

#define eps (1e-15)

static inline int solve(struct SolveData s_data, struct ThreadData* p_t_data,
                        double* V1, double* V2, double* V3) {
    // итераторы
    int i, j, r, q;
    // данные потока
    int th_num = p_t_data->k;
    int n_thrd = p_t_data->p;
    // данные задачи
    const int n = s_data.n;
    const int m = s_data.m;
    const int k = n / m;
    const int l = n - k * m;
    // длина/ширина текущего блока
    int av, ah;
    double * const A = s_data.A;
    double *pa, *pi, *pj;
    // счетчик необратимых матриц в столбце
    int c = 0;
    double ERROR = full_norm(p_t_data, s_data) * eps;
    (void) av, (void)ah, (void)pi, (void)pj, (void)j, (void) r, (void) q, (void) c;
    (void) th_num, (void)n_thrd, (void)i, (void)j, (void)pa, (void) l;
    (void) V1, (void) V2, (void) V3, (void) A;
    (void) ERROR;
    // LOG_DBL(ERROR);
    // for (j = 0; j * m < n; j++) {
    //     av = ah = j < k ? m : l;
    //     pa = A + th_num * n * m + j * av * m;

    //     // copy && identity
    //     for (r = 0; r < av; r++) {
    //         for (q = 0; q < ah; q++) {
    //             V1[r * ah + q] = pa[r * ah + q];
    //             V3[r * ah + q] = (r == q);
    //         }
    //     }
    // }

    return 0;
}

#endif  // SOLVE_H
