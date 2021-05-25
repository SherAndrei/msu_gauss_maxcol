#include "thread_func.h"
#include <sys/sysinfo.h>
#include <sched.h>
#include <unistd.h>
#include "error.h"
#include "print.h"
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
    args_t* a = (args_t*)ptr;
    cpu_set_t cpu;
    int m = a->slve_data.m;
    int n = a->slve_data.n;
    int nprocs = get_nprocs();
    CPU_ZERO(&cpu);
    CPU_SET((nprocs - 1 - a->thrd_data.k) % nprocs, &cpu);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);

    find_n_workable_lines(&a->thrd_data, a->slve_data);

    if (fill(&a->thrd_data, a->main_data, a->slve_data) < 0)
        return 0;
    if (a->thrd_data.n_workable_lines > 0) {
        a->assist.V1 = (double*)malloc(m * m * sizeof(double));
        a->assist.V2 = (double*)malloc(m * m * sizeof(double));
        a->assist.V3 = (double*)malloc(m * m * sizeof(double));
        a->assist.Buffer = (double*)malloc(m * n * sizeof(double));
        if (!a->assist.V1 || !a->assist.V2 ||
            !a->assist.V3 || !a->assist.Buffer) {
            a->thrd_data.error = ALLOC_ERROR;
        }
    } else {
        a->assist.V1 = 0;
        a->assist.V2 = 0;
        a->assist.V3 = 0;
        a->assist.Buffer = 0;
    }
    collect_errors(&a->thrd_data);
    if (a->thrd_data.error != 0) {
        free(a->assist.V1);
        free(a->assist.V2);
        free(a->assist.V3);
        free(a->assist.Buffer);
        return 0;
    }

    if (a->thrd_data.n_workable_lines > 0) {
        memset(a->assist.V1, 0, m * m * sizeof(double));
        memset(a->assist.V2, 0, m * m * sizeof(double));
        memset(a->assist.V3, 0, m * m * sizeof(double));
        memset(a->assist.Buffer, 0, m * n * sizeof(double));
    }
    if (a->thrd_data.k == 0) {
        printf("\nInitital matrix:\n");
        print_matrix(a->slve_data.A, n, n, m, a->main_data.r);
    }
    barrier(&a->thrd_data);
    a->time_data.cpu = get_time();
    a->time_data.wall_clock = get_full_time();
    n = solve(a, a->assist.V1, a->assist.V2, a->assist.V3,
              a->assist.Buffer);
    a->time_data.cpu = get_time() - a->time_data.cpu;
    a->time_data.wall_clock = get_full_time() - a->time_data.wall_clock;

    if (n != 0) {
        a->thrd_data.error = INAPL_ALG;
    }

    free(a->assist.V1);
    free(a->assist.V2);
    free(a->assist.V3);
    free(a->assist.Buffer);
    barrier(&a->thrd_data);
    return 0;
}
