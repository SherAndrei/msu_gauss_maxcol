#ifndef THREAD_FUNC_H
#define THREAD_FUNC_H
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

static inline double get_time() {
  struct rusage buf;
  getrusage(RUSAGE_THREAD, &buf);
  return (double)buf.ru_utime.tv_sec + (double)buf.ru_utime.tv_usec/1000000.;
}

static inline double get_full_time() {
  struct timeval buf;
  gettimeofday(&buf, 0);
  return (double)buf.tv_sec + (double)buf.tv_usec/1000000.;
}

void* thread_func(void* ptr);

#endif  // THREAD_FUNC_H

