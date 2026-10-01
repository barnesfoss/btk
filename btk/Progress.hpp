// Copyright Kameron Barnes
// SPDX-License-Identifier: MIT

#pragma once
#include "stdint.h"

#include <iostream>
#include <string>
#include <vector>

namespace btk {
namespace progress {
/// Represents a progress bar.
class Bar {
public:
  /// Creates a progress bar with a character length of \size and a maximum
  /// value of \max.
  ///
  /// \param size The size in characters of the progress bar.
  /// \param max The maximum value in which progess can reach.
  Bar(int size, int max) : size(size), max(max) {};
  friend std::ostream &operator<<(std::ostream &out, Bar &bar);

  /// Sets the progress of the bar
  ///
  /// /param prog How far the bar has progressed towards \c max.
  void setProgress(float prog) {
    progress = prog;
  }

  /// Sets the progress bar characters.
  ///
  /// Sets the character progression for the progress bar from least filled to
  /// filled.
  ///
  /// \param chars A vector of strings that represent the progression.
  void setCharacters(std::vector<std::string> chars) {
    characters = chars;
    maxCharacter = chars.size() - 1;
  };

  /// Toggles the percentage display to the right of the \c Bar.
  void togglePercent() {
    showPercent = !showPercent;
  }

  /// Toggles the brackets surrounding the \c Bar.
  void toggleBrackets() {
    showBrackets = !showBrackets;
  }

  /// Prints a \c Bar to the supplied output stream.
  ///
  /// \param out The target output stream.
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
        int characterIndex = static_cast<int>(position * (maxCharacter + 1));
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

/// Outputs the \c Bar using \c Bar::display.
///
/// \param out The output stream.
/// \param bar The \c Bar to output.
/// \returns A reference to \p out.
///
/// \see Bar::display
inline std::ostream &operator<<(std::ostream &out, Bar &bar) {
  bar.display(out);
  return out;
}

/// A progress bar with a spinner.
///
/// A derivative of the default \ref Bar that adds a spinner on the right side
/// of the display.
class Spinner : public Bar {
public:
  /// Creates a progress bar with a character length of \size and a maximum
  /// value of \max.
  ///
  /// \param size The size in characters of the progress bar.
  /// \param max The maximum value in which progess can reach.
  Spinner(int size, int max) : Bar(size, max) {}

  /// Prints a \c Spinner to the supplied output stream.
  ///
  /// \param out The target output stream.
  void display(std::ostream &out) override {
    out << spinner[tick] << " ";
    tick++;
    if (tick > spinnerLength)
      tick = 0;
    Bar::display(out);
  };

  /// Sets the character progression for the \c Spinner.
  ///
  /// The animated \c Spinner has a progression of characters it goes
  /// through.
  ///
  /// \param chars A vector of strings representing the animation.
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
