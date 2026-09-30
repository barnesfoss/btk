// Copyright Kameron Barnes
// SPDX-License-Identifier: MIT

#pragma once
#include <random>

namespace btk {
/// A pseudorandom number generator
///
/// Generates a number between a specificed minimum and maximum
class Random {
public:
  /// Creates a pseudorandom number generator with the specified range.
  ///
  /// \param min The minimum number that can be generated.
  /// \param max The maximum number that can be generated.
  Random(int min = 0, int max = 1) : twister(makeTwister()), dis(min, max) {}

  /// Creates a pseudorandom number generator with the specified range and
  /// seed
  ///
  /// \param seed The inital entropy used by the generator
  /// \param The minimum number that can be genrated.
  /// \param The maximum number that can be generated.
  Random(uint32_t seed, int min = 0, int max = 1) : twister(seed), dis(min, max) {};

  /// Generates the next number in the sequence.
  ///
  /// \returns A number in the range [min, max].
  int next() {
    return dis(twister);
  }

private:
  /// Makes a randomly initialized Mersenne Twister
  ///
  /// Creates a Mersenne Twister engine seeded with entropy from
  /// \c std::random_device
  ///
  /// \see std::random_device
  /// \return The generated Mersenne Twister engine
  static std::mt19937 makeTwister() {
    std::random_device rd;
    std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    return std::mt19937(seq);
  }

  std::mt19937 twister;
  std::uniform_int_distribution<int> dis;
};
} // namespace btk
