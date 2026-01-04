#pragma once
#include "FourWheelAngularSpeed.h"
#include <geometry_msgs/msg/twist.h>

/**
 * FWAngularSpeedWriter class for calculating wheel speeds and robot movement
 * data.
 *
 * This class provides methods to convert robot movement data to wheel speeds
 * and vice versa, using the kinematic equations for a Mecanum wheeled robot.
 */
class FWAngularSpeedWriter {
private:
  float r;
  float k;
  FourWheelAngularSpeed *angularSpeed;

public:
  /**
   * @brief Constructor for FWAngularSpeedWriter.
   *
   * @param l Distance between the front and rear wheels.
   * @param w Distance between the left and right wheels.
   * @param r Wheel radius.
   * @param angularSpeed Pointer to the FourWheelAngularSpeed object.
   */
  FWAngularSpeedWriter(float l, float w, float r,
                       FourWheelAngularSpeed *angularSpeed);

  /**
   * @brief Convert robot movement data to wheel speeds.
   *
   * Calculates the required angular speed for each wheel based on the desired
   * robot twist (linear and angular velocity) and writes it to the
   * FourWheelAngularSpeed object.
   *
   * @param twist Desired robot movement (linear x, y and angular z).
   */
  void write(geometry_msgs__msg__Twist *twist);

  /**
   * @brief Get the angular speed object.
   *
   * @return Reference to the FourWheelAngularSpeed object.
   */
  FourWheelAngularSpeed &getAngularSpeed() const;
};