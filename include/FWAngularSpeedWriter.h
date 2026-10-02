#pragma once
#include "FourWheelAngularSpeed.h"
#include <geometry_msgs/msg/twist.h>

/**
 * Base interface for wheel speed computation strategies.
 * Each strategy implements a different kinematic model.
 */
/** @brief Interface that converts chassis velocity into wheel angular speeds. */
class AngularSpeedStrategy {
public:
  /** @brief Destroy a strategy through its base pointer. */
  virtual ~AngularSpeedStrategy() = default;
  /**
   * @brief Compute wheel angular speeds for a chassis velocity command.
   * @param vx Linear velocity along the x axis.
   * @param vy Linear velocity along the y axis.
   * @param wz Angular velocity around the z axis.
   * @param out Destination wheel-speed structure, updated in place.
   */
  virtual void compute(float vx, float vy, float wz,
                        FourWheelAngularSpeed &out) = 0;
};

/**
 * Mecanum kinematics strategy.
 * For robots with Mecanum wheels (e.g. indoor robot).
 */
/** @brief Computes wheel speeds for a four-wheel mecanum drive. */
class MecanumStrategy : public AngularSpeedStrategy {
private:
  float r;
  float k;

public:
  /**
   * @brief Construct a mecanum-drive kinematics strategy.
   * @param l Half-length from robot center to wheel axle.
   * @param w Half-width from robot center to wheel.
   * @param r Wheel radius.
   */
  MecanumStrategy(float l, float w, float r);
  /** @copydoc AngularSpeedStrategy::compute(float, float, float, FourWheelAngularSpeed&) */
  void compute(float vx, float vy, float wz,
               FourWheelAngularSpeed &out) override;
};

/**
 * Skid-steer kinematics strategy.
 * For robots with fixed wheels and different front/back radii
 * (e.g. outdoor robot).
 */
/** @brief Computes wheel speeds for a skid-steer drive. */
class SkidSteerStrategy : public AngularSpeedStrategy {
private:
  float l;
  float r_front;
  float r_back;

public:
  /**
   * @brief Construct a skid-steer kinematics strategy.
   * @param l Distance between front and rear wheel axles.
   * @param r_front Front wheel radius.
   * @param r_back Rear wheel radius.
   */
  SkidSteerStrategy(float l, float r_front, float r_back);
  /** @copydoc AngularSpeedStrategy::compute(float, float, float, FourWheelAngularSpeed&) */
  void compute(float vx, float vy, float wz,
               FourWheelAngularSpeed &out) override;
};

/**
 * Converts Twist messages to wheel angular speeds using a pluggable strategy.
 */
/** @brief Applies a wheel-speed strategy to incoming ROS Twist commands. */
class FWAngularSpeedWriter {
private:
  AngularSpeedStrategy *strategy;
  FourWheelAngularSpeed *angularSpeed;

public:
  /**
   * @brief Construct a Twist-to-wheel-speed writer.
   * @param strategy Kinematics strategy retained for this writer's lifetime.
   * @param angularSpeed Output structure updated by write().
   */
  FWAngularSpeedWriter(AngularSpeedStrategy *strategy,
                       FourWheelAngularSpeed *angularSpeed);

  /**
   * @brief Convert a ROS Twist command into wheel angular speeds.
   * @param twist Command to convert; must remain valid for this call.
   */
  void write(geometry_msgs__msg__Twist *twist);
  /** @brief Access wheel angular speeds from the last write().
   * @return Reference to caller-provided output storage.
   */
  FourWheelAngularSpeed &getAngularSpeed() const;
};
