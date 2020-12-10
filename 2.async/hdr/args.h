#ifndef ARGS_H
#define ARGS_H
#include <pthread.h>

struct Time {
    double cpu;
    double wall_clock;
};

struct MainData {
    int s;
    const char* filename;
};

struct ThreadData {
    int k;
    int p;
    int result;
    int error;
};

struct SolveData {
    double* A;
    double* B;
    double* X;
    int n;
    int m;
};

struct Args {
    pthread_barrier_t* p_barrier;
    struct Time       time;
    struct ThreadData thrd_data;
    struct MainData   main_data;
    struct SolveData  slve_data;
};

#endif  // ARGS_H
