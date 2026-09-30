// Copyright Kameron Barnes
// SPDX-License-Identifier: MIT

#pragma once
#include <iostream>
#include <stdint.h>

namespace btk {
/// Represents a Color
///
/// Stores the red, green, and blue components as 8-bit unsigned integers.
/// The components are publicly accessible.
class Color {
public:
  /// Creates an RGB color from the specified red, green, and blue values.
  ///
  /// \param r The red component.
  /// \param g The green component.
  /// \param b The blue component.
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};

  /// Performs linear interpolation between two colors
  ///
  /// Takes this color, a provided \p target color, and an \p alpha value and
  /// creates a new Color using the lerped values
  ///
  /// \param target The target Color
  /// \param alpha The progress towards the target
  /// \returns A new interpolated Color
  Color lerp(const Color &target, float alpha) {
    return Color(
        r + (target.r - r) * alpha, g + (target.g - g) * alpha, b + (target.b - b) * alpha);
  }

  friend std::ostream &operator<<(std::ostream &out, Color &color);

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

/// Outputs the color in a human-readable format.
///
/// \param out The output stream.
/// \param color The color to format.
inline std::ostream &operator<<(std::ostream &out, Color &color) {
  out << "Color(" << (int)color.r << "," << (int)color.g << "," << (int)color.b << ")";
  return out;
};
} // namespace btk
