/*
 * Copyright Kameron Barnes
 * SPDX-License-Identifier: MIT
 * File: Unit.h
 * Tiny & standalone C89 unit tester
 */

#ifndef BTK_TESTSUITE_UNIT_H
#define BTK_TESTSUITE_UNIT_H

/* This is written in C, so C++ will need to define it as an extern to make the compiler not
 * complain */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************/
/* Structures */
/*******************************************************************/
typedef struct {
  int failed;
  const char *message;
  int line;
  const char *file;
} TestResult;

typedef struct {
  TestResult (*function)(void);
  const char *name;
} Test;

typedef struct {
  const char *name;
  int testCount;
  int failed;
  int testSize;
  Test *tests;
} Unit;

/*******************************************************************/
/* Functions */
/*******************************************************************/

/* Helper functions for allocation error checks */
void *__salloc(size_t n) {
  void *p = malloc(n);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
    abort();
  }
  return p;
}

void *__sralloc(void *original, size_t n) {
  void *p = realloc(original, n);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
    abort();
  }
  return p;
}

Unit *createUnit(const char *name) {
  /* We need to add these together */
  int size = sizeof(name) + sizeof(Unit);
  Unit *n = (Unit *)__salloc(size);
  n->name = name;
  n->testCount = 0;
  n->failed = 0;
  n->testSize = 0;
  return n;
}

void __addTest(Unit *unit, Test test) {
  /* Add the size of the new test to the current size */
  int size_old = unit->testSize;
  int size = size_old + sizeof(test);
  unit->tests = (Test *)__sralloc(unit->tests, size);
  /* Add the new values to the reallocated array */
  unit->tests[unit->testCount] = test;
  unit->testCount++;
  unit->testSize = size;
}

int runTests(Unit *unit) {
  /* We return this at the end */
  int result;

  /* Allocate space for the results */
  TestResult *results = (TestResult *)__salloc(sizeof(TestResult) * unit->testCount);

  /* Iterator */
  int i = unit->testCount;

  while (i-- > 0) {
    /* Call the function and get the TestResult */
    Test t = unit->tests[i];
    TestResult result = (t.function)();
    results[i] = result;
    if (result.failed) {
      /* If one test fails the whole unit fails */
      unit->failed = 1;
    }
  }

  /* Print whether the unit succeeded */
  if (unit->failed) {
    printf("\x1b[31m[-] %s\x1b[0m\n", unit->name);
  } else {
    printf("\x1b[32m[+] %s\x1b[0m\n", unit->name);
  }

  /* Print each test result */
  while (i++ < unit->testCount - 1) {
    TestResult result = results[i];
    Test t = unit->tests[i];
    if (result.failed) {
      printf("\t\x1b[31m[-] %s at line %i in %s with:\n\t\t%s\x1b[0m\n",
             t.name,
             result.line,
             result.file,
             result.message);
    } else {
      printf("\t\x1b[32m[+] %s\x1b[0m\n", t.name);
    }
  }

  /* Move here to avoid UAF */
  result = unit->failed;

  /* Free after the unit has been run */
  free(unit->tests);
  free(unit);
  free(results);
  return result;
}
#ifdef __cplusplus
}
#endif

/*******************************************************************/
/* Macros */
/*******************************************************************/
#define ADDTEST(unit, func)                                                                        \
  do {                                                                                             \
    Test t = {func, #func};                                                                        \
    __addTest(unit, t);                                                                            \
  } while (0)

#define RESULT(failed, message)                                                                    \
  do {                                                                                             \
    TestResult result = {failed, message, __LINE__, __FILE__};                                     \
    return result;                                                                                 \
  } while (0)

#define TESTEND                                                                                    \
  do {                                                                                             \
    TestResult result = {0};                                                                       \
    return result;                                                                                 \
  } while (0)

#define TEST(name) TestResult name()

#define ASSERT_EQ(a, b)                                                                            \
  do {                                                                                             \
    if (!((a) == (b))) {                                                                           \
      RESULT(1, "EQ assertion failed\n\t\ta==b\n\t\ta=" #a "\n\t\tb=" #b);                         \
    }                                                                                              \
  } while (0)

#define ASSERT_NEQ(a, b)                                                                           \
  do {                                                                                             \
    if (!((a) != (b))) {                                                                           \
      RESULT(1, "NEQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                        \
    }                                                                                              \
  } while (0)

#define ASSERT_TRUE(a)                                                                             \
  do {                                                                                             \
    if (!(a)) {                                                                                    \
      RESULT(1, "TRUE assertion failed\n\t\t" #a);                                                 \
    }                                                                                              \
  } while (0)

#define ASSERT_FALSE(a)                                                                            \
  do {                                                                                             \
    if (a) {                                                                                       \
      RESULT(1, "FALSE assertion failed\n\t\t" #a);                                                \
    }                                                                                              \
  } while (0)

#define ASSERT_NULL(a)                                                                             \
  do {                                                                                             \
    if ((a) != NULL) {                                                                             \
      RESULT(1, "NULL assertion failed\n\t\t" #a);                                                 \
    }                                                                                              \
  } while (0)

#define ASSERT_NNULL(a)                                                                            \
  do {                                                                                             \
    if ((a) == NULL) {                                                                             \
      RESULT(1, "NNULL assertion failed\n\t\t" #a);                                                \
    }                                                                                              \
  } while (0)

#define ASSERT_GT(a, b)                                                                            \
  do {                                                                                             \
    if (!((a) > (b))) {                                                                            \
      RESULT(1, "GT assertion failed\n\t\ta>b\n\t\ta=" #a "\n\t\tb=" #b);                          \
    }                                                                                              \
  } while (0)

#define ASSERT_GTE(a, b)                                                                           \
  do {                                                                                             \
    if (!((a) >= (b))) {                                                                           \
      RESULT(1, "GTE assertion failed\n\t\ta>=b\n\t\ta=" #a "\n\t\tb=" #b);                        \
    }                                                                                              \
  } while (0)

#define ASSERT_LT(a, b)                                                                            \
  do {                                                                                             \
    if (!((a) < (b))) {                                                                            \
      RESULT(1, "LT assertion failed\n\t\ta<b\n\t\ta=" #a "\n\t\tb=" #b);                          \
    }                                                                                              \
  } while (0)

#define ASSERT_LTE(a, b)                                                                           \
  do {                                                                                             \
    if (!((a) <= (b))) {                                                                           \
      RESULT(1, "LTE assertion failed\n\t\ta<=b\n\t\ta=" #a "\n\t\tb=" #b);                        \
    }                                                                                              \
  } while (0)

#define ASSERT_STR_EQ(a, b)                                                                        \
  do {                                                                                             \
    if (strcmp((a), (b)) != 0) {                                                                   \
      RESULT(1, "STR_EQ assertion failed\n\t\ta==b\n\t\ta=" #a "\n\t\tb=" #b);                     \
    }                                                                                              \
  } while (0)

#define ASSERT_STR_NEQ(a, b)                                                                       \
  do {                                                                                             \
    if (strcmp((a), (b)) == 0) {                                                                   \
      RESULT(1, "STR_NEQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                    \
    }                                                                                              \
  } while (0)

#define ASSERT_MEM_EQ(a, b)                                                                        \
  do {                                                                                             \
    if (memcmp((a), (b), (size)) == 0) {                                                           \
      RESULT(1, "MEM_EQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                     \
    }                                                                                              \
  } while (0)

#define ASSERT_MEM_NEQ(a, b)                                                                       \
  do {                                                                                             \
    if (memcmp((a), (b), (size)) == 0) {                                                           \
      RESULT(1, "MEM_NEQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                    \
    }                                                                                              \
  } while (0)

#define UNIMPLEMENTED RESULT(1, "This feature is unimplemented.");

#endif
