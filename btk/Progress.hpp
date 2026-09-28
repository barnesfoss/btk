#pragma once
#include "stdint.h"
#include <iostream>
#include <string>
namespace btk {
namespace progress {

/**
 * Creates a printable progress bar
 *
 * @param size The size in characters of the bar
 * @param max The maximum number the bar's `progress` reaches
 */
class Bar {
public:
  Bar(int size, int max) : size(size), max(max) {};
  friend std::ostream &operator<<(std::ostream &out, Bar &bar);

  /**
   * Sets the internal progress value of the `Bar`
   *
   * @param prog How far the bar has progressed to `max`
   */
  void setProgress(float prog) { progress = prog; }
  /**
   * Sets the character displayed on the filled part of the `Bar`
   *
   * @param c The character to set to
   */
  void setFilled(const char c) { filled = c; }
  /**
   * Sets the character displayed on the empty part of the `Bar`
   *
   * @param c The character to set to
   */
  void setEmpty(const char c) { empty = c; }
  /**
   * Toggles the percentage displayed to the right of the `Bar`
   */
  void togglePercent() { showPercent = !showPercent; }

  /**
   * Prints a `Bar` to the supplied ostream
   *
   * @param out Target ostream
   */
  virtual void display(std::ostream &out) {
    int prog = (progress / max) * size;
    out << "[" << std::string(prog, filled) << std::string(size - prog, empty)
        << "]";
    if (showPercent) {
      out << " " << progress / max * 100 << "%";
    }
  }

private:
  float progress = 0;
  float max;
  int size;
  char filled = '#';
  char empty = '-';
  bool showPercent = true;
};

/**
 * An overload of the << operator that calls display
 *
 * @param out The target ostream
 * @param bar A reference to the bar being printed
 */
inline std::ostream &operator<<(std::ostream &out, Bar &bar) {
  bar.display(out);
  return out;
};

/**
 * A derivative of the default Bar, adds a spinner on the right size of progress
 * bar
 *
 * @param size The size in characters of the bar
 * @param max The maximum number the bar's `progress` reaches
 */
class Spinner : public Bar {
public:
  Spinner(int size, int max) : Bar(size, max) {}
  /**
   * Prints a `Spinner` to the supplied `ostream`
   *
   * @param out Target ostream
   */
  void display(std::ostream &out) override {
    out << spinner[tick] << " ";
    tick++;
    if (tick > 3)
      tick = 0;
    Bar::display(out);
  };

private:
  char spinner[4] = {'\\', '|', '/', '-'};
  uint8_t tick = 0;
};
} // namespace progress
} // namespace btk
