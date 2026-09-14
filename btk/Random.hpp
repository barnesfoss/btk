#pragma once
#include <random>
namespace btk {
/**
 * A random number generator class.
 *
 * Generates a number between `min` and `max` at initialization using a Mersenne
 * Twister
 */
class Random {
public:
  /**
   * Default constructor for the `Random` class
   *
   * Description.
   *
   * @param min The minimum number generated; Int.
   * @param max The maximum number generated; Int.
   * @param seed The seed used by the Mersenne Twister, optional; Int.
   *
   */
  Random(int min = 0, int max = 1, int seed = rand()) {
    twister = std::mt19937(seed);
    dis = std::uniform_int_distribution<int>(min, max);
  };

  /**
   * Generates the next random number from the `Random` instance
   *
   * Returns an integer between the min and max defined at initialization
   *
   * @return A random Int
   */
  int next() { return dis(twister); }

private:
  std::mt19937 twister;
  std::uniform_int_distribution<int> dis;
};
} // namespace btk
