#include "WToPWMConverter.h"

WToPWMConverter::WToPWMConverter(float maxW, int minPWM, int maxPWM) {
  this->maxW = maxW;
  this->minPWM = minPWM;
  this->maxPWM = maxPWM;
}

int WToPWMConverter::convert(float w) {
  if (std::abs(w) < 0.01)
    return 0;

  float ratio = std::abs(w) / this->maxW;

  // Map that percentage to PWM range [PwmMin, PwmMax]
  int targetPWM = constrain(minPWM + ratio * (maxPWM - minPWM), minPWM, maxPWM);

  // Return PWM with correct sign
  return (w >= 0) ? targetPWM : -targetPWM;
}