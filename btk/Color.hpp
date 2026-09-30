// Copyright Kameron Barnes 2026
//
// Licensed under the MIT License
// See LICENSE for details
#ifndef BTK_COLOR_HPP
#define BTK_COLOR_HPP
#include <stdint.h>

#include <iostream>
namespace btk {
/** A class that stores a color via RGB
 *
 * @param r Red
 * @param g Green
 * @param b Blue
 */
class Color {
   public:
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};

    /** Performs linear interpolation between two colors
     *
     * @param target A target Color
     * @param alpha The progress towards the target
     */
    Color lerp(const Color& target, float alpha) {
        return Color(r + (target.r - r) * alpha, g + (target.g - g) * alpha,
                     b + (target.b - b) * alpha);
    }

    friend std::ostream& operator<<(std::ostream& out, Color& color);

    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/** An overload that outputs the string in a readable format
 *
 * @param out The target ostream
 * @param color The color that is being formatted
 */
inline std::ostream& operator<<(std::ostream& out, Color& color) {
    out << "Color(" << (int)color.r << "," << (int)color.g << ","
        << (int)color.b << ")";
    return out;
};
}  // namespace btk
#endif
