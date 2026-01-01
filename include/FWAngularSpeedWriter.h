#pragma once
#include "FourWheelAngularSpeed.h"
#include <geometry_msgs/msg/twist.h>

/**
 * FWAngularSpeedWriter class for calculating wheel speeds and robot movement data.
 *
 * This class provides methods to convert robot movement data to wheel speeds
 * and vice versa, using the kinematic equations for a Mecanum wheeled robot.
 */
class FWAngularSpeedWriter {
private:
  float r;
  float k;
  FWAngularSpeed *speed;

public:
  /**
   * Constructor for FWAngularSpeedWriter.
   * @param l Distance between the front and rear wheels
   * @param w Distance between the left and right wheels
   * @param r Wheel radius
   */
  FWAngularSpeedWriter(float l, float w, float r, FourWheelAngularSpeed *speed);

  /**
   * Convert robot movement data to wheel speeds.
   * @param rd Robot movement data
   * @param speeds Pointer to fwAngularSpeed object to store the calculated
   * wheel speeds
   */
  void write(geometry_msgs__msg__Twist *twist);
};