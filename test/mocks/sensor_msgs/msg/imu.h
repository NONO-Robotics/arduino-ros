#pragma once

struct sensor_msgs__msg__Quaternion {
  double x = 0;
  double y = 0;
  double z = 0;
  double w = 0;
};

struct sensor_msgs__msg__Vector3 {
  double x = 0;
  double y = 0;
  double z = 0;
};

struct sensor_msgs__msg__Imu {
  sensor_msgs__msg__Quaternion orientation;
  sensor_msgs__msg__Vector3 angular_velocity;
  sensor_msgs__msg__Vector3 linear_acceleration;
};
