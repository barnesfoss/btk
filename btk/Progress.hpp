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
   * Sets the character progression for the progress bar from least filled to
   * filled
   *
   * @params chars a vector of strings
   */
  void setCharacters(std::vector<std::string> chars) {
    characters = chars;
    maxCharacter = chars.size() - 1;
  };

  /**
   * Toggles the percentage displayed to the right of the `Bar`
   */
  void togglePercent() { showPercent = !showPercent; }

  /**
   * Toggles the percentage displayed to the right of the `Bar`
   */
  void toggleBrackets() { showBrackets = !showBrackets; }

  /**
   * Prints a `Bar` to the supplied ostream
   *
   * @param out Target ostream
   */
  virtual void display(std::ostream &out) {
    float percentage = progress / max;
    float prog = percentage * size;
    if (showBrackets)
      out << "[";
    for (int i = 0; i < size; i++) {
      float position = prog - i;
      if (position >= 1.0f) {
        out << characters[maxCharacter];
      } else if (position > 0.0f) {
        int characterIndex = (int)(position * (maxCharacter + 1));
        characterIndex = std::min(characterIndex, maxCharacter);
        out << characters[characterIndex];
      } else {
        out << characters[0];
      }
    }
    if (showBrackets)
      out << "]";
    if (showPercent)
      out << " " << percentage * 100 << "%";
  }

private:
  float progress = 0;
  float max;
  int size;
  std::vector<std::string> characters = {"-", "*", "#"};
  int maxCharacter = characters.size() - 1;
  bool showPercent = true;
  bool showBrackets = true;
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
  /**
   * Sets the character progression for the spinner
   *
   * @params chars a vector of strings
   */
  void setSpinner(std::vector<std::string> spin) {
    spinner = spin;
    spinnerLength = spin.size() - 1;
  }

private:
  std::vector<std::string> spinner = {"\\", "|", "/", "-"};
  uint spinnerLength = 3;
  uint8_t tick = 0;
};
} // namespace progress
} // namespace btk
