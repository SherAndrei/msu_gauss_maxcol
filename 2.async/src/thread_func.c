#include "thread_func.h"
#include <sys/sysinfo.h>
#include <sched.h>
#include <unistd.h>
#include "fill.h"
#include "args.h"
#include "solve.h"

void* thread_func(void* ptr) {
    struct Args* a = (struct Args*)ptr;
    cpu_set_t cpu;
    int res;
    int nprocs = get_nprocs();
    CPU_ZERO(&cpu);
    CPU_SET((nprocs - 1 - a->thrd_data.k) % nprocs, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);

    if (fill(&a->thrd_data, a->main_data, a->slve_data) < 0)
        return 0;
    // TODO ERRROR FILE
    pthread_barrier_wait(a->p_barrier);

    a->time.cpu = get_time();
    a->time.wall_clock = get_full_time();
    res = solve(a->slve_data, a->thrd_data);
    a->time.cpu = get_time() - a->time.cpu;
    a->time.wall_clock = get_full_time() - a->time.wall_clock;

    if (res != 0) {
        a->thrd_data.error = -1;
    }
    pthread_barrier_wait(a->p_barrier);
    return 0;
}
