#include "VelocityConverter.h"

VelocityConverter::VelocityConverter(float maxWInRadSeg,
                                     int pwmResolutionInBits, int minPwm) {
  this->maxWInRadSeg = maxWInRadSeg;
  // Calculate max value based on bits (e.g. 12 bits -> 4095)
  this->maxPwm = (int)((1UL << pwmResolutionInBits) - 1);
  this->minPwm = minPwm;
};

int VelocityConverter::wToSignedPWM(float w) {
  // Absolute software deadzone (zero noise)
  if (abs(w) < 0.01)
    return 0;

  // Limit input to not exceed robot physics
  if (abs(w) > maxWInRadSeg)
    w = getSign(w) * maxWInRadSeg;

  // Mapping calculation with floating point arithmetic for precision
  // PWM = ( |Current Omega| / Max Omega ) * MaxPWM_Counts
  float pwm = (abs(w) / maxWInRadSeg) * (float)maxPwm;

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

int VelocityConverter::getSign(float value) { return value > 0 ? 1 : -1; }