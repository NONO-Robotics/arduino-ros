#pragma once
#include "FourWheelAngularSpeed.h"
#include <geometry_msgs/msg/twist.h>

/**
 * Base interface for wheel speed computation strategies.
 * Each strategy implements a different kinematic model.
 */
class AngularSpeedStrategy {
public:
  virtual ~AngularSpeedStrategy() = default;
  virtual void compute(float vx, float vy, float wz,
                        FourWheelAngularSpeed &out) = 0;
};

/**
 * Mecanum kinematics strategy.
 * For robots with Mecanum wheels (e.g. indoor robot).
 */
class MecanumStrategy : public AngularSpeedStrategy {
private:
  float r;
  float k;

public:
  MecanumStrategy(float l, float w, float r);
  void compute(float vx, float vy, float wz,
               FourWheelAngularSpeed &out) override;
};

/**
 * Skid-steer kinematics strategy.
 * For robots with fixed wheels and different front/back radii
 * (e.g. outdoor robot).
 */
class SkidSteerStrategy : public AngularSpeedStrategy {
private:
  float l;
  float r_front;
  float r_back;

public:
  SkidSteerStrategy(float l, float r_front, float r_back);
  void compute(float vx, float vy, float wz,
               FourWheelAngularSpeed &out) override;
};

/**
 * Converts Twist messages to wheel angular speeds using a pluggable strategy.
 */
class FWAngularSpeedWriter {
private:
  AngularSpeedStrategy *strategy;
  FourWheelAngularSpeed *angularSpeed;

public:
  FWAngularSpeedWriter(AngularSpeedStrategy *strategy,
                       FourWheelAngularSpeed *angularSpeed);

  void write(geometry_msgs__msg__Twist *twist);
  FourWheelAngularSpeed &getAngularSpeed() const;
};
