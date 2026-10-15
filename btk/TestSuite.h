/*
 * Copyright Kameron Barnes
 * SPDX-License-Identifier: MIT
 * Part of the btk library
 *
 * File: TestSuite.h
 * Catch-all include header for the TestSuite
 */

#ifndef BTK_TESTSUITE

#define BTK_TESTSUITE
#ifdef BTK_TESTSUITE_IMPL
#define BTK_UNIT_IMPL
#endif
#include "TestSuite/Benchmark.h"
#include "TestSuite/Unit.h"

#if defined(__cplusplus) & !defined(BTK_TESTSUITE_NOEXT)
#include "TestSuite/Unit.hpp"
#endif

#endif
