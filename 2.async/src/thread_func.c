#include "thread_func.h"
#include <sys/sysinfo.h>
#include <sched.h>
#include <unistd.h>
#include "error.h"
#include "fill.h"
#include "synchronise.h"
#include "solve.h"

static inline void find_n_workable_lines(thread_data_t* p_t_data, solve_data_t sl_d) {
    int n_lines = sl_d.n / sl_d.m + ((sl_d.n % sl_d.m) > 0);
    int n_thrd  = p_t_data->p;
    int n_thread_lines = n_lines / n_thrd;
    int remainder = n_lines - n_thrd * n_thread_lines;

    p_t_data->n_workable_lines = n_thread_lines + (p_t_data->k < remainder);
}


void* thread_func(void* ptr) {
    struct Args* a = (struct Args*)ptr;
    cpu_set_t cpu;
    int res;
    int nprocs = get_nprocs();
    double *V1, *V2, *V3;
    CPU_ZERO(&cpu);
    CPU_SET((nprocs - 1 - a->thrd_data.k) % nprocs, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);

    find_n_workable_lines(&a->thrd_data, a->slve_data);

    if (fill(&a->thrd_data, a->main_data, a->slve_data) < 0)
        return 0;
    if (a->thrd_data.n_workable_lines > 0) {
        V1 = (double*)malloc(a->slve_data.m * a->slve_data.m);
        V2 = (double*)malloc(a->slve_data.m * a->slve_data.m);
        V3 = (double*)malloc(a->slve_data.m * a->slve_data.m);
        if (!V1 || !V2 || !V3) {
            free(V1), free(V2), free(V3);
            a->thrd_data.error = ALLOC_ERROR;
        }
    }
    collect_errors(&a->thrd_data);
    if (a->thrd_data.error != 0) {
        return 0;
    }

    a->time_data.cpu = get_time();
    a->time_data.wall_clock = get_full_time();
    res = solve(a->slve_data, &a->thrd_data, V1, V2, V3);
    a->time_data.cpu = get_time() - a->time_data.cpu;
    a->time_data.wall_clock = get_full_time() - a->time_data.wall_clock;

    if (res != 0) {
        a->thrd_data.error = INAPL_ALG;
    }
    if (a->thrd_data.n_workable_lines > 0) {
        free(V1), free(V2), free(V3);
    }
    pthread_barrier_wait(a->p_barrier);
    return 0;
}
