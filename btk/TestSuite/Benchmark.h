// Copyright Kameron Barnes
// SPDX-License-Identifier: MIT

#pragma once
#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#define BENCH_MEASURE(func, ...)                                                                   \
  struct timespec start, end;                                                                      \
  clock_gettime(CLOCK_MONOTONIC, &start);                                                          \
  (func)(__VA_ARGS__);                                                                             \
  clock_gettime(CLOCK_MONOTONIC, &end);

#define BENCH_MS(func, ...)                                                                        \
  do {                                                                                             \
    BENCH_MEASURE(func, __VA_ARGS__)                                                               \
    int64_t diff = (((int64_t)end.tv_sec) * 1000 + ((int64_t)end.tv_nsec) / 1000000) -             \
                   (((int64_t)start.tv_sec) * 1000 + ((int64_t)start.tv_nsec) / 1000000);          \
    printf(#func " took %lu ms\n", diff);                                                          \
  } while (0)

#define BENCH_NS(func, ...)                                                                        \
  do {                                                                                             \
    BENCH_MEASURE(func, __VA_ARGS__);                                                              \
    int64_t diff = ((int64_t)end.tv_sec * 1000000000LL + end.tv_nsec) -                            \
                   ((int64_t)start.tv_sec * 1000000000LL + start.tv_nsec);                         \
                                                                                                   \
    printf(#func " took %lld ns (%.3f us)\n", (long long)diff, (double)diff / 1000.0);             \
  } while (0)

#define BENCH_NS_N(func, n, ...)                                                                   \
  do {                                                                                             \
    BENCH_MEASURE(func, __VA_ARGS__);                                                              \
                                                                                                   \
    int64_t total = ((int64_t)end.tv_sec * 1000000000LL + end.tv_nsec) -                           \
                    ((int64_t)start.tv_sec * 1000000000LL + start.tv_nsec);                        \
                                                                                                   \
    printf(#func ": %lld ns total, %.3f ns/call\n", (long long)total, (double)total / (n));        \
  } while (0)
