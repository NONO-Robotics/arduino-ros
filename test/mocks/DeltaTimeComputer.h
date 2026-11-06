#pragma once

#include <Arduino.h>

class DeltaTimeComputer {
 public:
  explicit DeltaTimeComputer(unsigned long interval) : interval_(interval) {}

  void reset() { lastUpdate_ = millis(); }
  void update() {}
  bool hasBeenReached() const { return millis() - lastUpdate_ >= interval_; }

 private:
  unsigned long interval_;
  unsigned long lastUpdate_ = 0;
};
