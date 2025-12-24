#pragma once
#include "FWAngularSpeed.h"
#include <geometry_msgs/msg/twist.h>

/**
 * MecanumKinematics class for calculating wheel speeds and robot movement data.
 *
 * This class provides methods to convert robot movement data to wheel speeds
 * and vice versa, using the kinematic equations for a Mecanum wheeled robot.
 */
class MecanumKinematics {
private:
  float r;
  float k;

public:
  /**
   * Constructor for MecanumKinematics.
   * @param l Distance between the front and rear wheels
   * @param w Distance between the left and right wheels
   * @param r Wheel radius
   */
  MecanumKinematics(float l, float w, float r);

  /**
   * Convert robot movement data to wheel speeds.
   * @param rd Robot movement data
   * @param speeds Pointer to fwAngularSpeed object to store the calculated
   * wheel speeds
   */
  void twistTofwAngularSpeed(geometry_msgs__msg__Twist *twist,
                             FWAngularSpeed *speed);
};