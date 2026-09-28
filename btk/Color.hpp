#pragma once
#include <iostream>
#include <stdint.h>
namespace btk {

class Color {
public:
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};
  Color lerp(const Color &target, float alpha) {
    return Color(r + (target.r - r) * alpha, g + (target.g - g) * alpha,
                 b + (target.b - b) * alpha);
  }
  friend std::ostream &operator<<(std::ostream &out, Color &color);
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

inline std::ostream &operator<<(std::ostream &out, Color &color) {
  out << "Color(" << (int)color.r << "," << (int)color.g << "," << (int)color.b
      << ")";
  return out;
};
} // namespace btk
