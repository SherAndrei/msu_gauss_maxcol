#ifndef ARGS_H
#define ARGS_H
#include <pthread.h>

typedef struct TimeData {
    double cpu;
    double wall_clock;
} time_data_t;

typedef struct MainData {
    int s;
    const char* filename;
} main_data_t;

typedef struct ThreadData {
    int k;
    int p;
    int n_workable_lines;
    int result;
    int error;
    double norm;
} thread_data_t;

typedef struct SolveData {
    double* A;
    double* B;
    double* X;
    int n;
    int m;
} solve_data_t;

typedef struct Args {
    pthread_barrier_t* p_barrier;
    struct TimeData   time_data;
    struct ThreadData thrd_data;
    struct MainData   main_data;
    struct SolveData  slve_data;
} args_t;

#endif  // ARGS_H
