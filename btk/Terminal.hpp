#pragma once
#include <cstdlib>
#include <string.h>
namespace btk {
/**
 * Returns whether a terminal has truecolor support
 */
inline bool isTrueColor() {
  char *result = std::getenv("COLORTERM");
  return strcmp(result, "truecolor") == 0;
};
} // namespace btk
