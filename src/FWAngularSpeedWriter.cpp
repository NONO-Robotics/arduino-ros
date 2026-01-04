#include "FWAngularSpeedWriter.h"

FWAngularSpeedWriter::FWAngularSpeedWriter(
  float l, 
  float w, 
  float r, 
  FourWheelAngularSpeed *angularSpeed) {
  this->r = r;
  this->k = l + w;
  this->angularSpeed = angularSpeed;
}

void FWAngularSpeedWriter::write(
  geometry_msgs__msg__Twist *twist
) {
  float vx = twist->linear.x;
  float vy = twist->linear.y;
  float wz = twist->angular.z;

  float fl = (1.0f / r) * (vx - vy - k * wz);
  float fr = (1.0f / r) * (vx + vy + k * wz);
  float bl = (1.0f / r) * (vx + vy - k * wz);
  float br = (1.0f / r) * (vx - vy + k * wz);

  angularSpeed->updateFrom(fl, fr, bl, br);
}

FourWheelAngularSpeed& FWAngularSpeedWriter::getAngularSpeed() const {
  return *angularSpeed;
}