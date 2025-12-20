#pragma once
#include <Arduino.h>

/**
 * @brief Converts robot physical limits and PWM resolution into control values.
 */
class VelocityConverter {
private:
  float maxWInRadSeg;
  int minPwm, maxPwm;

public:
  /**
   * @brief Constructor.
   * @param maxWInRadSeg Maximum physical angular velocity of the robot
   * @param pwmResolutionInBits PWM resolution (Recommended: 12 bits)
   * @param minPwm Minimum deadzone for motor to start turning
   */
  VelocityConverter(float maxWInRadSeg, int pwmResolutionInBits, int minPwm);

  /**
   * @brief Convert angular velocity to signed PWM.
   * @param w Angular velocity.
   * @return Signed PWM value.
   */
  int wToSignedPWM(float w);

private:
  int getSign(float value);
};