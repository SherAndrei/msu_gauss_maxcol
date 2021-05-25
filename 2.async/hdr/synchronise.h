#ifndef SYNCHRO_H
#define SYNCHRO_H
#include <string.h>
#include <pthread.h>
#include "args.h"

static inline void __barrier_impl(struct ThreadData* cur,
                                  void (*start)(struct ThreadData**, struct ThreadData**),
                                  void (*finish)(struct ThreadData**, struct ThreadData**)) {
    static pthread_mutex_t m     = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in   = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static struct ThreadData* first = 0;
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = cur;
    } else if (start != 0) {
        (*start)(&cur, &first);
    }
    t_in++;
    if (t_in >= cur->p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < cur->p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (first != cur && finish != 0) {
        (*finish)(&cur, &first);
    }
    t_out++;
    if (t_out >= cur->p) {
        t_in  = 0;
        first = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < cur->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
}

static inline void __collect_errors_start(struct ThreadData** p_cur,
                                          struct ThreadData** p_first) {
    if ((*p_cur)->error != 0) {
        (*p_first)->error = (*p_cur)->error;
    }
}

static inline void __collect_errors_finish(struct ThreadData** p_cur,
                                           struct ThreadData** p_first) {
    (*p_cur)->error = (*p_first)->error;
}

static inline void collect_errors(struct ThreadData* th_data) {
    __barrier_impl(th_data, __collect_errors_start, __collect_errors_finish);
}

static inline void __find_norm_start(struct ThreadData** p_cur,
                                     struct ThreadData** p_first) {
    if ((*p_cur)->norm > (*p_first)->norm) {
        (*p_first)->norm = (*p_cur)->norm;
    }
}

static inline void __find_norm_finish(struct ThreadData** p_cur,
                                      struct ThreadData** p_first) {
    (*p_cur)->norm = (*p_first)->norm;
}

static inline void find_norm(struct ThreadData* th_data) {
    __barrier_impl(th_data, __find_norm_start, __find_norm_finish);
}

static inline void find_max(thread_data_t* cur,
                            double* thread_matr, double thread_norm, int m,
                            int* p_line_num) {
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in   = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static thread_data_t* first = 0;
    static double* matrix = 0;
    static double  norm   = 0.;
    static int     line_n = 0;
    pthread_mutex_lock(&mutex);
    if (first == 0) {
        first = cur;
        matrix = thread_matr;
        norm = thread_norm;
        line_n = *p_line_num;
    } else {
        first->result += cur->result;
        if (cur->n_workable_lines > 0) {
            if (thread_norm < norm) {
                norm = thread_norm;
                matrix = thread_matr;
                line_n = *p_line_num;
            }
        }
    }
    t_in++;
    if (t_in >= cur->p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < cur->p) {
            pthread_cond_wait(&c_in, &mutex);
        }
    }
    if (cur != first) {
        cur->result = first->result;
    }
    if (cur->n_workable_lines > 0 && thread_matr != matrix) {
        memcpy(thread_matr, matrix, m * m * sizeof(double));
        *p_line_num =  line_n;
    }
    t_out++;
    if (t_out >= cur->p) {
        t_in  = 0;
        first = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < cur->p) {
            pthread_cond_wait(&c_out, &mutex);
        }
    }
    pthread_mutex_unlock(&mutex);
}

static inline void barrier(thread_data_t* cur) {
    __barrier_impl(cur, NULL, NULL);
}

#endif  // SYNCHRO_H

