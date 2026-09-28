#pragma once
#include <cstdlib>
#include <stdint.h>
#include <string>
namespace btk {

class TermColor {
public:
  TermColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};
  std::string toEscape() {
    char buf[50];
    snprintf(buf, sizeof(buf), "\x1b[38;2;%i;%i;%im", r, g, b);
    return std::string(buf);
  }

  TermColor lerp(const TermColor &target, float alpha) {
    return TermColor(r + (target.r - r) * alpha, g + (target.g - g) * alpha,
                     b + (target.b - b) * alpha);
  }

  friend std::ostream &operator<<(std::ostream &out, TermColor &color);

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

inline char TERMCOLOR_RESET[] = "\x1b[0m";

inline std::ostream &operator<<(std::ostream &out, TermColor &color) {
  out << color.toEscape();
  return out;
};
/**
 * Returns whether a terminal has truecolor support
 */
inline bool isTrueColor() {
  std::string result = std::getenv("COLORTERM");
  return result == "truecolor";
};
} // namespace btk
