#define _GNU_SOURCE
#include "thread_func.h"
#include <sys/sysinfo.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>


struct Args {
    int k;
    int p;
}

void* thread_func(void* ptr) {
    struct Args* a = (struct Args*)ptr;
    cpu_set_t cpu;
    int k = a->k;
    int p = a->p;
    int nprocs = get_nprocs();
    CPU_ZERO(&cpu);
    CPU_SET(nprocs - 1 - k, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);
    
}
