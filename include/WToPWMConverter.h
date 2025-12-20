#pragma once
#include <Arduino.h>

/**
 * WToPWMConverter class for converting angular velocity (rad/s) to PWM values.
 *
 * This class provides a method to convert a desired angular velocity for a wheel
 * into a corresponding PWM value that can be used to control a DC motor.
 */
class WToPWMConverter
{
private:
    float maxW;
    int minPWM;
    int maxPWM;

public:
    /**
     * Constructor for WToPWMConverter.
     *
     * @param maxW Maximum angular velocity of a wheel (rad/s)
     */
    WToPWMConverter(float maxW, int minPWM, int maxPWM);

    /**
     * Converts a target speed (rad/s) from PID output to a raw PWM value using linear mapping.
     * This function assumes the target speed is within the range [-maxW, maxW] as limited by the PID output.
     * It maps the absolute value of the target speed linearly to the range [0, pwmMax].
     * The sign of the target speed is preserved in the output PWM.
     *
     * @param w The target speed in radians per second, typically the output of a PID controller.
     * @return A raw PWM value (signed) in the range [-pwmMax, pwmMax].
     */
    int convert(float w);
};