// Copyright Kameron Barnes 2026
//
// Licensed under the MIT License
// See LICENSE for details
#ifndef BTK_TERMINAL_HPP
#define BTK_TERMINAL_HPP
#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "Color.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#endif

namespace btk {

class TermColor : public Color {
   public:
    TermColor(uint8_t r, uint8_t g, uint8_t b) : Color(r, g, b) {};
    TermColor(const Color& color) : Color(color.r, color.g, color.b) {};

    /** Formats a string to a terminal escape sequence
     *
     * @return Escaped string
     */
    std::string toEscape() {
        char buf[20];
        snprintf(buf, sizeof(buf), "\x1b[38;2;%i;%i;%im", r, g, b);
        return std::string(buf);
    }

    friend std::ostream& operator<<(std::ostream& out, TermColor& color);
};

inline char TERMCOLOR_RESET[] = "\x1b[0m";

/** An overload that outputs the string in a terminal color code
 *
 * @param out The target ostream
 * @param color The color that is being formatted
 */
inline std::ostream& operator<<(std::ostream& out, TermColor& color) {
    out << color.toEscape();
    return out;
};

/** Returns whether a terminal has truecolor support
 */
inline bool isTrueColor() {
    std::string result = std::getenv("COLORTERM");
    return result == "truecolor";
};

/** Sets the cursor visibility in the terminal
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

}  // namespace btk
#endif
