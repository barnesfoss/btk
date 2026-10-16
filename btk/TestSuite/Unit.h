/*
 * Copyright Kameron Barnes
 * SPDX-License-Identifier: MIT
 * Part of the btk library
 *
 * File: Unit.h
 * Tiny & standalone C89 unit tester
 */

#ifndef BTK_TESTSUITE_UNIT_H
#define BTK_TESTSUITE_UNIT_H
#include <memory.h>
#include <stdio.h> // IWYU pragma: export
#include <stdlib.h>
#include <string.h> // IWYU pragma: export

/*******************************************************************/
/* Structures */
/*******************************************************************/

/* Represents a result of a test. */
typedef struct {
  char failed;
  const char *message;
  int line;
  const char *file;
  char amessage; // Whether the message was dynamically allocated
                 // We need to check this to be sure we are freeing something that can actually be
                 // freed since the new optimizations allocate memory for these
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
  char failed;
  size_t testSize;
  Test *tests;
} Unit;

/*******************************************************************/
/* Function Definitions*/
/*******************************************************************/
/* This is written in C, so C++ will need to define it as an extern to make the compiler not
 * complain */
#ifdef __cplusplus
extern "C" {
#endif
/* \brief Internal helper function for allocation error checks.
 *
 * Calls malloc and checks success, if not successful abort and throw an error message.
 *
 * \param n The size to allocate.
 * \returns A pointer to the allocated memory.
 */
// void *__salloc(size_t n);

/* \brief Internal helper function for allocation error checks.
 *
 * Calls realloc and checks success, if not successful abort and throw an error message.
 *
 * \param n The size to allocate.
 * \returns A pointer to the allocated memory.
 */
// void *__sralloc(void *original, size_t n);

/* \brief Creates a new Unit test in memory.
 *
 * A constructor for the \c Unit struct.
 *
 * \param name The name of the unit you are creating.
 * \returns A newly initialized \c Unit.
 */
Unit *createUnit(const char *name);

/* \brief Internal function that adds a test to a \c Unit.
 *
 * Allocates memory for a new test within a \c Unit and adds it to the list.
 *
 * \param unit The unit.
 * \param test The test added to the unit.
 */
void __addTest(Unit *unit, Test test);

/* \brief Runs all the tests within the \c Unit.
 *
 * All tests will be run in sequence and output to the terminal, this also destructs the Unit.
 *
 * \param unit The unit to run.
 */
int runTests(Unit *unit);

TestResult __result(char failed,
                    char type,
                    char comparison,
                    const char *a,
                    const char *b,
                    size_t line,
                    const char *file,
                    const char *extra);

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
 * \param failed Whether the test fails or not (True or False).
 * \param message Associated error message.
 */
#define RESULT(failed, message)                                                                    \
  do {                                                                                             \
    TestResult result = {failed, message, __LINE__, __FILE__, 0};                                  \
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
#define ASSERT_EQ_MSG(a, b, extra)                                                                 \
  do {                                                                                             \
    if (!((a) == (b))) {                                                                           \
      return __result(1, 0, 0, #a, #b, __LINE__, __FILE__, extra);                                 \
    }                                                                                              \
  } while (0)
#define ASSERT_EQ(a, b) ASSERT_EQ_MSG(a, b, NULL)

/* Asserts whether two values are not equivalent, returns a failed TestResult if not.*/
#define ASSERT_NEQ_MSG(a, b, extra)                                                                \
  do {                                                                                             \
    if (!((a) != (b))) {                                                                           \
      return __result(1, 1, 1, #a, #b, __LINE__, __FILE__, extra);                                 \
    }                                                                                              \
  } while (0)
#define ASSERT_NEQ(a, b) ASSERT_NEQ_MSG(a, b, NULL)

/* Asserts whether a value is true, returns a failed TestResult if not.*/
#define ASSERT_TRUE_MSG(a, extra)                                                                  \
  do {                                                                                             \
    if (!(a)) {                                                                                    \
      return __result(1, 2, 0, #a, FAIL_STRINGS[1], __LINE__, __FILE__, extra);                    \
    }                                                                                              \
  } while (0)
#define ASSERT_TRUE(a) ASSERT_TRUE_MSG(a, NULL)

/* Asserts whether a value is false, returns a failed TestResult if not.*/
#define ASSERT_FALSE_MSG(a, extra)                                                                 \
  do {                                                                                             \
    if (a) {                                                                                       \
      return __result(1, 3, 0, #a, FAIL_STRINGS[0], __LINE__, __FILE__, extra);                    \
    }                                                                                              \
  } while (0)
#define ASSERT_FALSE(a) ASSERT_FALSE_MSG(a, NULL)

/* Asserts whether a value is NULL, returns a failed TestResult if not.*/
#define ASSERT_NULL_MSG(a, extra)                                                                  \
  do {                                                                                             \
    if ((a) != NULL) {                                                                             \
      return __result(1, 4, 0, #a, FAIL_STRINGS[2], __LINE__, __FILE__, extra);                    \
    }                                                                                              \
  } while (0)
#define ASSERT_NULL(a) ASSERT_NULL_MSG(a, NULL)

/* Asserts whether a value is not NULL, returns a failed TestResult if not.*/
#define ASSERT_NNULL_MSG(a, extra)                                                                 \
  do {                                                                                             \
    if ((a) == NULL) {                                                                             \
      return __result(1, 5, 1, #a, FAIL_STRINGS[2], __LINE__, __FILE__, extra);                    \
    }                                                                                              \
  } while (0)
#define ASSERT_NNULL(a) ASSERT_NNULL_MSG(a, NULL)

/* Asserts whether a value is greater than the other, returns a failed TestResult if not.*/
#define ASSERT_GT_MSG(a, b, extra)                                                                 \
  do {                                                                                             \
    if (!((a) > (b))) {                                                                            \
      return __result(1, 6, 4, #a, #b, __LINE__, __FILE__, extra);                                 \
    }                                                                                              \
  } while (0)
#define ASSERT_GT(a, b) ASSERT_GT_MSG(a, b, NULL)

/* Asserts whether a value is greater than or equal to the other, returns a failed TestResult if
 * not.*/
#define ASSERT_GTE_MSG(a, b, extra)                                                                \
  do {                                                                                             \
    if (!((a) >= (b))) {                                                                           \
      return __result(1, 7, 5, #a, #b, __LINE__, __FILE__, extra);                                 \
    }                                                                                              \
  } while (0)
#define ASSERT_GTE(a, b) ASSERT_GTE_MSG(a, b, NULL)

/* Asserts whether a value is less than than the other, returns a failed TestResult if not.*/
#define ASSERT_LT_MSG(a, b, extra)                                                                 \
  do {                                                                                             \
    if (!((a) < (b))) {                                                                            \
      return __result(1, 8, 6, #a, #b, __LINE__, __FILE__, extra);                                 \
    }                                                                                              \
  } while (0)
#define ASSERT_LT(a, b) ASSERT_LT_MSG(a, b, NULL)

/* Asserts whether a value is less than than or equal to the other, returns a failed TestResult if
 * not.*/
#define ASSERT_LTE_MSG(a, b, extra)                                                                \
  do {                                                                                             \
    if (!((a) <= (b))) {                                                                           \
      return __result(1, 9, 7, #a, #b, __LINE__, __FILE__, extra);                                 \
    }                                                                                              \
  } while (0)
#define ASSERT_LTE(a, b) ASSERT_LTE_MSG(a, b, NULL)

/* Asserts whether two strings are equivalent, returns a failed TestResult if not.*/
#define ASSERT_STR_EQ_MSG(a, b, extra)                                                             \
  do {                                                                                             \
    if (strcmp((a), (b))) {                                                                        \
      return __result(1, 10, 0, #a, #b, __LINE__, __FILE__, extra);                                \
    }                                                                                              \
  } while (0)
#define ASSERT_STR_EQ(a, b) ASSERT_STR_EQ_MSG(a, b, NULL)

/* Asserts whether two strings are not equivalent, returns a failed TestResult if not.*/
#define ASSERT_STR_NEQ_MSG(a, b, extra)                                                            \
  do {                                                                                             \
    if (!strcmp((a), (b))) {                                                                       \
      return __result(1, 11, 1, #a, #b, __LINE__, __FILE__, extra);                                \
    }                                                                                              \
  } while (0)
#define ASSERT_STR_NEQ(a, b) ASSERT_STR_NEQ_MSG(a, b, NULL)

/* Asserts whether two memory objects are equivalent, returns a failed TestResult if not.*/
#define ASSERT_MEM_EQ_MSG(a, b, size, extra)                                                       \
  do {                                                                                             \
    if (memcmp((a), (b), (size))) {                                                                \
      return __result(1, 12, 0, #a, #b, __LINE__, __FILE__, extra);                                \
    }                                                                                              \
  } while (0)
#define ASSERT_MEM_EQ(a, b, size) ASSERT_MEM_EQ_MSG(a, b, size, NULL)

/* Asserts whether two memory objects not equivalent, returns a failed TestResult if not.*/
#define ASSERT_MEM_NEQ_MSG(a, b, size, extra)                                                      \
  do {                                                                                             \
    if (!memcmp((a), (b), (size))) {                                                               \
      return __result(1, 13, 1, #a, #b, __LINE__, __FILE__, extra);                                \
    }                                                                                              \
  } while (0)
#define ASSERT_MEM_NEQ(a, b, size) ASSERT_MEM_NEQ_MSG(a, b, size, NULL)

/* Returns a failing result saying "This feature is unimplemented". */
#define UNIMPLEMENTED RESULT(1, "This feature is unimplemented.");

#endif
// #define BTK_UNIT_IMPL
#ifdef BTK_UNIT_IMPL
#undef BTK_UNIT_IMPL
/* This is written in C, so C++ will need to define it as an extern to make the compiler not
 * complain */
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************/
/* Function Implementations*/
/*******************************************************************/

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

const char *ASSERT_NAMES[14] = {"EQ",
                                "NEQ",
                                "TRUE",
                                "FALSE",
                                "NULL",
                                "NNULL",
                                "GT",
                                "GTE",
                                "LT",
                                "LTE",
                                "STR_EQ",
                                "STR_NEQ",
                                "MEM_EQ",
                                "MEM_NEQ"};
const char *FAIL_BASE = "%s assertion failed\n\t\ta%sb\n\t\ta=%s\n\t\tb=%s\n\t\t%s";
const char *FAIL_STRINGS[3] = {"false", "true", "NULL"};
const char *ASSERT_COMPSTR[6] = {"==", "!=", ">", ">=", "<", "<="};

TestResult __result(char failed,
                    char type,
                    char comparison,
                    const char *a,
                    const char *b,
                    size_t line,
                    const char *file,
                    const char *extra) {
  if (extra == NULL) {
    extra = "";
  }

  // Find how much space we need for the formatted string
  // FAIL_BASE contains 3 `%s` meaning we would have 6 extra bytes
  // We need to add 1 to account for null terminating byte
  size_t needed = strlen(FAIL_BASE) + strlen(a) + strlen(b) + strlen(ASSERT_NAMES[type]) +
                  strlen(extra) + strlen(ASSERT_COMPSTR[comparison]) - 5;
  char *message = (char *)__salloc(needed);

  sprintf(message, FAIL_BASE, ASSERT_NAMES[type], ASSERT_COMPSTR[comparison], a, b, extra);
  TestResult result = {failed, message, __LINE__, __FILE__, 1};

  return result;
};

Unit *createUnit(const char *name) {
  /* We need to add these together */
  int size = sizeof(Unit);
  Unit *n = (Unit *)__salloc(size);
  n->name = name;
  n->testCount = 0;
  n->failed = 0;
  n->testSize = 0;
  n->tests = NULL;
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
  }
#ifndef BTK_UNIT_HIDESUCCESS
  else {
    printf("\x1b[32m[+] %s\x1b[0m\n", unit->name);
  }
#endif

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
    }
#ifndef BTK_UNIT_HIDESUCCESS
    else {
      printf("\t\x1b[32m[+] %s\x1b[0m\n", t.name);
    }
#endif
    if (result.amessage) {
      free((char *)result.message);
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
#endif
