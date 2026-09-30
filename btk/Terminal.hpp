// Copyright Kameron Barnes
// SPDX-License-Identifier: MIT

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
/// Represents an RGB color for truecolor terminal output
class TermColor : public Color {
public:
  /// Creates an RGB color from the specified red, green, and blue values.
  ///
  /// \param r The red component.
  /// \param g The green component.
  /// \param b The blue component.
  TermColor(uint8_t r, uint8_t g, uint8_t b) : Color(r, g, b) {};

  /// Creates a terminal color from an existing color.
  ///
  /// \param color The color to convert.
  TermColor(const Color &color) : Color(color.r, color.g, color.b) {};

  /// Formats the color to a terminal control sequence
  ///
  /// \returns An escaped terminal control sequence containing the color
  std::string toControl() {
    char buf[20];
    snprintf(buf, sizeof(buf), "\x1b[38;2;%i;%i;%im", r, g, b);
    return std::string(buf);
  }

  friend std::ostream &operator<<(std::ostream &out, TermColor &color);
};

inline char TERMCOLOR_RESET[] = "\x1b[0m";

/// An overload that outputs the control sequence to the terminal
///
/// \param out The output stream
/// \param color The Color to be output
/// \see TermColor::toEscape
inline std::ostream &operator<<(std::ostream &out, TermColor &color) {
  out << color.toControl();
  return out;
}

/// Checks whether the terminal environment supports truecolor.
///
/// Examines the \c COLORTERM environment variable and returns whether there is
/// support.
///
/// \returns \c true if truecolor support is detected, otherwise \c false.
inline bool isTrueColor() {
  const char *colorTerm = std::getenv("COLORTERM");
  return colorTerm && std::string(colorTerm) == "truecolor";
}

/// Sets the terminal's cursor visibility
///
/// \param show Whether to show or hide the cursor
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
