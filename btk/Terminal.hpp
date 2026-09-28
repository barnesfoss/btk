#pragma once
#include "Color.hpp"
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#endif

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

/**
 * Sets the cursor visibility in the terminal
 *
 * @param show Whether to show or hide the cursor
 */
inline void showCursor(bool show) {
#if defined(_WIN32)
  static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(handle, &cci);
  cci.bVisible = show;
  SetConsoleCursorInfo(handle, &cci);
#else
  std::cout << (show ? "\033[?25h" : "\033[?25l");
#endif
}

} // namespace btk
