#ifndef SYNCHRO_H
#define SYNCHRO_H
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
    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = cur;
    } else {
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
    if (first != cur) {
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

static inline void __collect_errors_start(struct ThreadData** p_cur, struct ThreadData** p_first) {
    if ((*p_cur)->error != 0) {
        (*p_first)->error = (*p_cur)->error;
    }
}

static inline void __collect_errors_finish(struct ThreadData** p_cur, struct ThreadData** p_first) {
    (*p_cur)->error = (*p_first)->error;
}

static inline void collect_errors(struct ThreadData* th_data) {
    __barrier_impl(th_data, __collect_errors_start, __collect_errors_finish);
}

static inline void __find_norm_start(struct ThreadData** p_cur, struct ThreadData** p_first) {
    if ((*p_cur)->norm > (*p_first)->norm) {
        (*p_first)->norm = (*p_cur)->norm;
    }
}

static inline void __find_norm_finish(struct ThreadData** p_cur, struct ThreadData** p_first) {
    (*p_cur)->norm = (*p_first)->norm;
}

static inline void find_norm(struct ThreadData* th_data) {
    __barrier_impl(th_data, __find_norm_start, __find_norm_finish);
}

#endif  // SYNCHRO_H

