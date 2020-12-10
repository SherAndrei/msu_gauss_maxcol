#ifndef SOLVE_H
#define SOLVE_H
#include <stdlib.h>
#include "thread_func.h"
#include "gauss_inverse.h"

static inline int solve(struct SolveData s_data, struct ThreadData t_data /* TODO */) {
    // итераторы
    int i, j;
    // данные потока
    int th_num = t_data.k;
    int n_thrd = t_data.p;
    // данные задачи
    const int n = s_data.n;
    const int m = s_data.m;
    const int k = n / m;
    const int l = n - k * m;
    // длина/ширина текущего блока
    int av, ah;
    // диапозон работы потока
    // вспомогательные блоки
    double *V1, *V2, *V3;
    double *pa = s_data.A, *pi, *pj;

    (void) av, (void)ah, (void)pi, (void)pj, (void)j;
    (void) th_num, (void)n_thrd, (void)i, (void)j, (void)pa, (void) l;

    V1 = (double*)malloc(m * m);
    V2 = (double*)malloc(m * m);
    V3 = (double*)malloc(m * m);
    if (!V1 || !V2 || !V3) {
        // TODO error
        free(V1), free(V2), free(V3);
    }

    free(V1), free(V2), free(V3);
    return 0;
}

#endif  // SOLVE_H
