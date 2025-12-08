#include "FWKinematics.h"

FWKinematics::FWKinematics(
    float l,
    float w,
    float r)
{
    this->r = r;
    this->k = l + w;
}

void FWKinematics::twistTofwAngularSpeed(
    geometry_msgs__msg__Twist *twist,
    FWAngularSpeed *speed)
{
    float vx = twist->linear.x;
    float vy = twist->linear.y;
    float wz = twist->angular.z;

    float fl = (1.0f / r) * (vx - vy - k * wz); 
    float fr = (1.0f / r) * (vx + vy + k * wz);
    float bl = (1.0f / r) * (vx + vy - k * wz);
    float br = (1.0f / r) * (vx - vy + k * wz);

    speed->updateFrom(fl, fr, bl, br);
}