#pragma once

struct geometry_msgs__msg__Vector3 {
  double x = 0;
  double y = 0;
  double z = 0;
};

struct geometry_msgs__msg__Twist {
  geometry_msgs__msg__Vector3 linear;
  geometry_msgs__msg__Vector3 angular;
};
