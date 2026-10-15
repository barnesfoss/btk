/*
 * Copyright Kameron Barnes
 * SPDX-License-Identifier: MIT
 * Part of the btk library
 *
 * File: Unit.hpp
 * An extension of Unit.h, with an object oriented approach.
 */
#ifndef BTK_TESTSUITE_UNIT_HPP
#ifndef __cplusplus
#error Unit.hpp is made for C++, please use Unit.h
#endif
#define BTK_TESTSUITE_UNIT_HPP
#include "Unit.h"

#include <vector>
// Undefine TEST as given by Unit.h
#undef TEST

// Sets up macros for nice syntax and auto test registry
/// \brief Creates a test
/// Creates a test and automatically adds it to a unit's registry
/// MUST be used inside a \c UnitTest
/// \param name The name of the test
#define TEST(name)                                                                                 \
  static void register_##name(Derived &self) {                                                     \
    Test t = {name, #name};                                                                        \
    self.addTest(t);                                                                               \
  }                                                                                                \
  inline static const bool name##_registered =                                                     \
      btk::UnitTest<Derived>::registerTest(Derived::register_##name);                              \
  static TestResult name()

/// \brief Creates a UnitTest
/// Shorthand for \c UnitTest with the parts needed for operation
/// \param name The name of the Unit
#define UNIT(name)                                                                                 \
  class name : public btk::UnitTest<name> {                                                        \
    using Derived = name;                                                                          \
                                                                                                   \
  public:                                                                                          \
    name() : UnitTest(#name) {};                                                                   \
                                                                                                   \
  private:

/// \brief Shorthand for `};` this is for consistency as TEST() has an open bracket
#define UNITEND                                                                                    \
  }                                                                                                \
  ;

/// \brief Shorthand to run a unit
/// \param unit A \c UnitTest
#define UNITRUN(unit) unit().run()

// Definition
namespace btk {
template <class Derived> class UnitTest {
private:
  using TestFn = void (*)(Derived &);
  Unit *unit;
  bool ran = false;

  /// \brief Puts all the functions into the registry vector
  void registerTests() {
    for (auto fn : registry()) {
      fn(static_cast<Derived &>(*this));
    }
  }

  /// \brief Fetches the internal function registry vector
  /// \returns registry vector pointer
  static std::vector<TestFn> &registry() {
    static std::vector<TestFn> entries;
    return entries;
  }

public:
  explicit UnitTest() {
    free(unit->tests);
    free(unit);
  };

  explicit UnitTest(const char *name) : unit(createUnit(name)) {}

  /// \brief Runs all tests
  ///
  /// This also registers all of the tests but this is irrelevant to the end user
  /// The unit cannot be ran after it's already been run.
  ///
  int run() {
    if (ran)
      return 1;
    registerTests();
    ran = true;
    return runTests(this->unit);
  }

protected:
  /// \brief Pushes a test onto the registry
  /// \returns Always returns true
  static bool registerTest(TestFn fn) {
    registry().push_back(fn);
    return true;
  }

  /// \brief Adds a test to the internal Unit
  /// \param A \c Test object
  void addTest(Test test) {
    __addTest(unit, test);
  }
};
}; // namespace btk
#endif
