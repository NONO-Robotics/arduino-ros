#pragma once
#include <Arduino.h>

/**
 * @brief Converts robot physical limits and PWM resolution into control values.
 */
class WToSignedPWMConverter {
private:
  float maxW;
  int minPwm;
  int maxPwm;

public:
  /**
   * @brief Constructor.
   * @param maxW Maximum physical angular velocity of the robot
   * @param pwmResolutionInBits PWM resolution (Recommended: 12 bits)
   * @param minPwm Minimum deadzone for motor to start turning
   */
  WToSignedPWMConverter(float maxW, int pwmResolutionInBits, int minPwm);

  /**
   * @brief Convert angular velocity to signed PWM.
   * @param w Angular velocity.
   * @return Signed PWM value.
   */
  int convert(float w);

private:
  int getSign(float value);
};