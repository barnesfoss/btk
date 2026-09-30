// Copyright Kameron Barnes 2026
//
// Licensed under the MIT License
// See LICENSE for details
#ifndef BTK_RANDOM_HPP
#define BTK_RANDOM_HPP
#include <random>
namespace btk {
/** A random number generator class.
 *
 * Generates a number between `min` and `max` at initialization using a Mersenne
 * Twister
 */
class Random {
   public:
    /** Default constructor for the `Random` class
     *
     * Description.
     *
     * @param min The minimum number generated; Int.
     * @param max The maximum number generated; Int.
     *
     */
    Random(int min = 0, int max = 1) : twister(makeTwister()), dis(min, max) {};

    /** Constructor for the `Random` class
     *
     * @param seed A random seed to provide entropy to the mersene twister;
     * uint32_t.
     * @param min The minimum number generated; Int.
     * @param max The maximum number generated; Int.
     *
     */
    Random(uint32_t seed, int min = 0, int max = 1)
        : twister(seed), dis(min, max) {};

    /** Generates the next random number from the `Random` instance
     *
     * Returns an integer between the min and max defined at initialization
     *
     * @return A random Int
     */
    int next() { return dis(twister); }

   private:
    /** Makes a random Mersenne Twister
     * Creates a Mersenne Twister engine seeded with entropy from
     * `std::random_device`.
     *
     * @return A randomly seeded Mersenne Twister engine.
     */
    static std::mt19937 makeTwister() {
        std::random_device rd;
        std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        return std::mt19937(seq);
    }

    std::mt19937 twister;
    std::uniform_int_distribution<int> dis;
};
}  // namespace btk
#endif
