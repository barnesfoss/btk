#pragma once
#include "stdint.h"
#include <iostream>
#include <string>
#include <vector>
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
  void setFilled(std::string c) { filled = c; }
  /**
   * Sets the character displayed on the empty part of the `Bar`
   *
   * @param c The character to set to
   */
  void setEmpty(std::string c) { empty = c; }

  /**
   * Sets the character displayed between filled and empty on the `Bar`
   *
   * @param c The character to set to
   */
  void setMiddle(std::string c) { middle = c; }

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
    out << "[";
    for (int i = 0; i < prog - 1; i++) {
      out << filled;
    }
    if (prog != size and middle != "")
      out << middle;
    for (int i = 0; i < size - prog - 1; i++) {
      out << empty;
    }
    out << "]";
    if (showPercent) {
      out << " " << progress / max * 100 << "%";
    }
  }

private:
  float progress = 0;
  float max;
  int size;
  std::string filled = "#";
  std::string middle = "";
  std::string empty = "-";
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
    if (tick > spinnerLength)
      tick = 0;
    Bar::display(out);
  };

  void setSpinner(std::vector<std::string> spin) {
    spinner = spin;
    spinnerLength = spin.size() - 1;
  }

private:
  std::vector<std::string> spinner = {"\\", "|", "/", "-"};
  uint spinnerLength = 4;
  uint8_t tick = 0;
};
} // namespace progress
} // namespace btk
