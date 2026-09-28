#pragma once
#include "Color.hpp"
#include <cstdlib>
#include <stdint.h>
#include <string>
namespace btk {

class TermColor : public Color {
public:
  TermColor(uint8_t r, uint8_t g, uint8_t b) : Color(r, g, b) {};
  TermColor(const Color &color) : Color(color.r, color.g, color.b) {};
  std::string toEscape() {
    char buf[19];
    snprintf(buf, sizeof(buf), "\x1b[38;2;%i;%i;%im", r, g, b);
    return std::string(buf);
  }
  friend std::ostream &operator<<(std::ostream &out, TermColor &color);
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
