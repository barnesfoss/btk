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

/* Represents a result of a test. */
typedef struct {
  int failed;
  const char *message;
  int line;
  const char *file;
} TestResult;

/* A Test object containing the name and the function. */
typedef struct {
  TestResult (*function)(void);
  const char *name;
} Test;

/* Represents a Unit within a Unit test. */
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

/* \brief Internal helper function for allocation error checks.
 *
 * Calls malloc and checks success, if not successful abort and throw an error message.
 *
 * \param n The size to allocate.
 * \returns A pointer to the allocated memory.
 */
void *__salloc(size_t n) {
  void *p = malloc(n);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
    abort();
  }
  return p;
}

/* \brief Internal helper function for allocation error checks.
 *
 * Calls realloc and checks success, if not successful abort and throw an error message.
 *
 * \param n The size to allocate.
 * \returns A pointer to the allocated memory.
 */
void *__sralloc(void *original, size_t n) {
  void *p = realloc(original, n);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
    abort();
  }
  return p;
}

/* \brief Creats a new Unit test in memory.
 *
 * A constructor for the \c Unit struct.
 *
 * \param name The name of the unit you are creating.
 * \returns A newly initialized \c Unit.
 */
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

/* \brief Internal function that adds a test to a \c Unit.
 *
 * Allocates memory for a new test within a \c Unit and adds it to the list.
 *
 * \param unit The unit.
 * \param test The test added to the unit.
 */
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

/* \brief Runs all the tests within the \c Unit.
 *
 * All tests will be run in sequence and output to the terminal, this also destructs the Unit.
 *
 * \param unit The unit to run.
 */
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

/* \brief Abstraction for the \c __addTest function.
 *
 * Automatically creates a \c Test object when given a test defined with TEST().
 *
 * \param unit The unit object we are adding the test to.
 * \param func The function version of a test.
 */
#define ADDTEST(unit, func)                                                                        \
  do {                                                                                             \
    Test t = {func, #func};                                                                        \
    __addTest(unit, t);                                                                            \
  } while (0)

/* \brief Abstraction for a \c TestResult.
 *
 * Makes returning a \c TestResult simple by making it function-like call, and automatically inserts
 * extra data that the user shouldn't have to enter.
 *
 * \param failed Whether the test failes or not (True or False).
 * \param message Associated error message.
 */
#define RESULT(failed, message)                                                                    \
  do {                                                                                             \
    TestResult result = {failed, message, __LINE__, __FILE__};                                     \
    return result;                                                                                 \
  } while (0)

/* Returns a successful \c TestResult */
#define TESTEND                                                                                    \
  do {                                                                                             \
    TestResult result = {0};                                                                       \
    return result;                                                                                 \
  } while (0)

/* \brief Abstraction for a test function definition.
 *
 * Internally just represents a function with a \c TestResult return value
 *
 */
#define TEST(name) TestResult name()

/* Asserts whether two values are equivalent, returns a failed TestResult if not.*/
#define ASSERT_EQ(a, b)                                                                            \
  do {                                                                                             \
    if (!((a) == (b))) {                                                                           \
      RESULT(1, "EQ assertion failed\n\t\ta==b\n\t\ta=" #a "\n\t\tb=" #b);                         \
    }                                                                                              \
  } while (0)

/* Asserts whether two values are not equivalent, returns a failed TestResult if not.*/
#define ASSERT_NEQ(a, b)                                                                           \
  do {                                                                                             \
    if (!((a) != (b))) {                                                                           \
      RESULT(1, "NEQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                        \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is true, returns a failed TestResult if not.*/
#define ASSERT_TRUE(a)                                                                             \
  do {                                                                                             \
    if (!(a)) {                                                                                    \
      RESULT(1, "TRUE assertion failed\n\t\t" #a);                                                 \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is false, returns a failed TestResult if not.*/
#define ASSERT_FALSE(a)                                                                            \
  do {                                                                                             \
    if (a) {                                                                                       \
      RESULT(1, "FALSE assertion failed\n\t\t" #a);                                                \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is NULL, returns a failed TestResult if not.*/
#define ASSERT_NULL(a)                                                                             \
  do {                                                                                             \
    if ((a) != NULL) {                                                                             \
      RESULT(1, "NULL assertion failed\n\t\t" #a);                                                 \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is not NULL, returns a failed TestResult if not.*/
#define ASSERT_NNULL(a)                                                                            \
  do {                                                                                             \
    if ((a) == NULL) {                                                                             \
      RESULT(1, "NNULL assertion failed\n\t\t" #a);                                                \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is greater than the other, returns a failed TestResult if not.*/
#define ASSERT_GT(a, b)                                                                            \
  do {                                                                                             \
    if (!((a) > (b))) {                                                                            \
      RESULT(1, "GT assertion failed\n\t\ta>b\n\t\ta=" #a "\n\t\tb=" #b);                          \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is greater than or equal to the other, returns a failed TestResult if
 * not.*/
#define ASSERT_GTE(a, b)                                                                           \
  do {                                                                                             \
    if (!((a) >= (b))) {                                                                           \
      RESULT(1, "GTE assertion failed\n\t\ta>=b\n\t\ta=" #a "\n\t\tb=" #b);                        \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is less than than the other, returns a failed TestResult if not.*/
#define ASSERT_LT(a, b)                                                                            \
  do {                                                                                             \
    if (!((a) < (b))) {                                                                            \
      RESULT(1, "LT assertion failed\n\t\ta<b\n\t\ta=" #a "\n\t\tb=" #b);                          \
    }                                                                                              \
  } while (0)

/* Asserts whether a value is less than than or equal to the other, returns a failed TestResult if
 * not.*/
#define ASSERT_LTE(a, b)                                                                           \
  do {                                                                                             \
    if (!((a) <= (b))) {                                                                           \
      RESULT(1, "LTE assertion failed\n\t\ta<=b\n\t\ta=" #a "\n\t\tb=" #b);                        \
    }                                                                                              \
  } while (0)

/* Asserts whether two strings are equivalent, returns a failed TestResult if not.*/
#define ASSERT_STR_EQ(a, b)                                                                        \
  do {                                                                                             \
    if (strcmp((a), (b)) != 0) {                                                                   \
      RESULT(1, "STR_EQ assertion failed\n\t\ta==b\n\t\ta=" #a "\n\t\tb=" #b);                     \
    }                                                                                              \
  } while (0)

/* Asserts whether two strings are not equivalent, returns a failed TestResult if not.*/
#define ASSERT_STR_NEQ(a, b)                                                                       \
  do {                                                                                             \
    if (strcmp((a), (b)) == 0) {                                                                   \
      RESULT(1, "STR_NEQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                    \
    }                                                                                              \
  } while (0)

/* Asserts whether two memory objects are equivalent, returns a failed TestResult if not.*/
#define ASSERT_MEM_EQ(a, b)                                                                        \
  do {                                                                                             \
    if (memcmp((a), (b), (size)) == 0) {                                                           \
      RESULT(1, "MEM_EQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                     \
    }                                                                                              \
  } while (0)

/* Asserts whether two memory objects not equivalent, returns a failed TestResult if not.*/
#define ASSERT_MEM_NEQ(a, b)                                                                       \
  do {                                                                                             \
    if (memcmp((a), (b), (size)) == 0) {                                                           \
      RESULT(1, "MEM_NEQ assertion failed\n\t\ta!=b\n\t\ta=" #a "\n\t\tb=" #b);                    \
    }                                                                                              \
  } while (0)

/* Returns a failing result saying "This feature is unimplemented". */
#define UNIMPLEMENTED RESULT(1, "This feature is unimplemented.");

#endif
