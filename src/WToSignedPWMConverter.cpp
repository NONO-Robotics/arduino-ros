#include "WToSignedPWMConverter.h"

WToSignedPWMConverter::WToSignedPWMConverter(float maxW,
                                             int pwmResolutionInBits,
                                             int minPwm) {
  this->maxW = maxW;
  this->maxPwm = (int)((1UL << pwmResolutionInBits) - 1);
  this->minPwm = minPwm;
};

int WToSignedPWMConverter::convert(float w) {
  // Absolute software deadzone (zero noise)
  if (abs(w) < 0.01)
    return 0;

  // Limit input to not exceed robot physics
  if (abs(w) > maxW)
    w = getSign(w) * maxW;

  // Mapping calculation with floating point arithmetic for precision
  // PWM = ( |Current Omega| / Max Omega ) * MaxPWM_Counts
  float pwm = (abs(w) / maxW) * (float)maxPwm;

  // Motor Deadzone Compensation
  // If calculation gives 1 but motor needs 50 to move, we adjust.
  if (pwm > 0 && pwm < minPwm) {
    pwm = minPwm;
  }

  // Final safety clamp
  if (pwm > maxPwm)
    pwm = maxPwm;

  return getSign(w) * (int)pwm;
};

int WToSignedPWMConverter::getSign(float value) { return value > 0 ? 1 : -1; }