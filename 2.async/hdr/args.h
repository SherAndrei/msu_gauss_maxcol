#ifndef ARGS_H
#define ARGS_H
#include <pthread.h>

typedef struct TimeData {
    double cpu;
    double wall_clock;
} time_data_t;

typedef struct MainData {
    int r;
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

typedef struct Assist {
    double* V1;
    double* V2;
    double* V3;
    double* Buffer;
} assist_t;

typedef struct Args {
    time_data_t   time_data;
    thread_data_t thrd_data;
    main_data_t   main_data;
    solve_data_t  slve_data;
    assist_t assist;
    struct Args* begin;
    struct Args* end;
} args_t;

#endif  // ARGS_H
