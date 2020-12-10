#ifndef SYNCHRO_H
#define SYNCHRO_H
#include <pthread.h>
#include "args.h"

static inline void collect_errors(struct ThreadData* th_data) {
    static pthread_mutex_t m     = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in   = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static struct ThreadData * first = 0;
    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = th_data;
    } else {
        if (th_data->error != 0) {
            first->error = th_data->error;
        }
    }
    t_in++;
    if (t_in >= th_data->p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < th_data->p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (th_data != first) {
        th_data->error = first->error;
    }
    t_out++;
    if (t_out >= th_data->p) {
        t_in  = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < th_data->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
}

#endif  // SYNCHRO_H

