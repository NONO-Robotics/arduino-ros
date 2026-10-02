#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <geometry_msgs/msg/vector3_stamped.h>
#include <rosidl_runtime_c/string_functions.h>
#include <TinyGPS++.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <MicroRosTimeUtils.h>
#include <std_msgs/msg/string.h>

inline void resetMocks() {
  arduino_mock::reset();
  Wire.reset();
  tinygps_mock::reset();
  geometry_msgs_mock::reset();
  rosidl_mock::reset();
  rcl_mock::reset();
  rclc_mock::reset();
  time_mock::reset();
  std_msgs_string_mock::reset();
}
