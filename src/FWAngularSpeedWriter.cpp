#include "FWAngularSpeedWriter.h"

// --- MecanumStrategy ---

MecanumStrategy::MecanumStrategy(float l, float w, float r) {
  this->r = r;
  this->k = l + w;
}

void MecanumStrategy::compute(float vx, float vy, float wz,
                               FourWheelAngularSpeed &out) {
  float fl = (1.0f / r) * (vx - vy - k * wz);
  float fr = (1.0f / r) * (vx + vy + k * wz);
  float bl = (1.0f / r) * (vx + vy - k * wz);
  float br = (1.0f / r) * (vx - vy + k * wz);
  out.updateFrom(fl, fr, bl, br);
}

// --- SkidSteerStrategy ---

SkidSteerStrategy::SkidSteerStrategy(float l, float r_front, float r_back) {
  this->l = l;
  this->r_front = r_front;
  this->r_back = r_back;
}

void SkidSteerStrategy::compute(float vx, float vy, float wz,
                                 FourWheelAngularSpeed &out) {
  // Note: Right side motors have invertDirection() applied at hardware level,
  // so we negate wz to produce correct turning direction (positive wz = left turn)
  float v_left = vx + (l / 2.0f) * wz;
  float v_right = vx - (l / 2.0f) * wz;

  float fl = v_left / r_front;
  float fr = v_right / r_front;
  float bl = v_left / r_back;
  float br = v_right / r_back;
  out.updateFrom(fl, fr, bl, br);
}

// --- FWAngularSpeedWriter ---

FWAngularSpeedWriter::FWAngularSpeedWriter(
    AngularSpeedStrategy *strategy,
    FourWheelAngularSpeed *angularSpeed) {
  this->strategy = strategy;
  this->angularSpeed = angularSpeed;
}

void FWAngularSpeedWriter::write(geometry_msgs__msg__Twist *twist) {
  strategy->compute(twist->linear.x, twist->linear.y, twist->angular.z,
                    *angularSpeed);
}

FourWheelAngularSpeed &FWAngularSpeedWriter::getAngularSpeed() const {
  return *angularSpeed;
}
