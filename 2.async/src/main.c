/*
    Метод Гаусса с выбором максимального элемента по столбцу
    решения системы линейных уравнений
*/
#define _GNU_SOURCE
#include <stdlib.h>
#include <pthread.h>
#include "thread_func.h"
#include "args.h"
#include "error.h"
#include "print.h"
#include "matrix.h"

static inline double* alloc_matrix(const int size) {
    return (double*) malloc (size * sizeof(double));
}

static inline void free_matrix(double* matrix) {
    free(matrix);
}

int main(int argc, const char* argv[]) {
    const char* filename = 0;
    int n, p, m, r, s, k;
    struct Args * a;
    pthread_t* tids;
    pthread_barrier_t barrier;
    double *A = 0;
    double *B = 0, *X = 0;

    printf(" Usage: [");
    for (k = 1; k < argc - 1; k++)
        printf("%s, ", argv[k]);
    printf("%s]\n", argv[argc - 1]);

    if (!((argc == 6 || argc == 7) &&
        (sscanf(argv[1], "%d", &n) == 1) &&
        (sscanf(argv[2], "%d", &p) == 1) &&
        (sscanf(argv[3], "%d", &m) == 1) &&
        (sscanf(argv[4], "%d", &r) == 1) &&
        (sscanf(argv[5], "%d", &s) == 1) &&
        (n >  0) && (p >  0) &&
        (m >  0) && (m <= n) &&
        (r >= 0) && (r <= n) &&
        (s >= 0) && (s <= 4)))
            return error(1);

    A  = alloc_matrix(n * n);
    B  = alloc_matrix(n * 1);
    X  = alloc_matrix(n * 1);
    a      = (struct Args*) malloc(sizeof(struct Args) * p);
    tids   = (pthread_t*)   malloc(sizeof(pthread_t) * (p - 1));
    if (!A || !B || !X || !a || !tids) {
        free_matrix(A);
        free_matrix(B),  free_matrix(X);
        free(a), free(tids);
        return error(2);
    }

    if (s == 0)
        filename = argv[6];

    pthread_barrier_init(&barrier, NULL, p);

    for (k = 0; k < p; k++) {
        a[k].p_barrier = &barrier;
        a[k].thrd_data.k = k;
        a[k].thrd_data.p = p;
        a[k].thrd_data.error  = 0;
        a[k].thrd_data.result = 0;
        a[k].thrd_data.norm   = 0.;
        a[k].time_data.cpu = 0.;
        a[k].time_data.wall_clock = 0.;
        a[k].slve_data.A = A;
        a[k].slve_data.B = B;
        a[k].slve_data.X = X;
        a[k].slve_data.m = m;
        a[k].slve_data.n = n;
        a[k].main_data.s = s;
        a[k].main_data.filename = filename;
    }

    for (k = 1; k < p; k++) {
        if (pthread_create(tids + k - 1, 0, &thread_func, a + k)) {
            printf("Cannot create thread %d\n", k);
        }
    }
    thread_func(a + 0);

    for (k = 1; k < p; k++) {
        pthread_join(tids[k - 1], 0);
    }

    if (a[0].thrd_data.error != 0) {
        k = a[0].thrd_data.error;
        free_matrix(A);
        free_matrix(B);
        free_matrix(X);
        free(a);
        free(tids);
        return error(k);
    } else {
        if (r > 0) {
            print_matrix(A, n, n, m, r);
            print_matrix(B, n, 1, m, r);
        }
        printf("Norm: %e\n", a[0].thrd_data.norm);
    }

    free_matrix(A);
    free_matrix(B);
    free_matrix(X);
    free(a), free(tids);
    return 0;
}
