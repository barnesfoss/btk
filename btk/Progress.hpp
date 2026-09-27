#pragma once
#include "stdint.h"
#include <iostream>
#include <string>
namespace btk {
namespace progress {

class Bar {
public:
  Bar(int size, int max) : size(size), max(max) {};
  friend std::ostream &operator<<(std::ostream &out, Bar &bar);
  void setProgress(float prog) { progress = prog; }
  void setFilled(const char c) { filled = c; }
  void setEmpty(const char c) { empty = c; }
  void togglePercent() { showPercent = !showPercent; }
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
inline std::ostream &operator<<(std::ostream &out, Bar &bar) {
  bar.display(out);
  return out;
};

class Spinner : public Bar {
public:
  Spinner(int size, int max) : Bar(size, max) {}
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
