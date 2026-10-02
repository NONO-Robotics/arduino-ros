#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <geometry_msgs/msg/vector3_stamped.h>
#include <rosidl_runtime_c/string_functions.h>
#include <TinyGPS++.h>

inline void resetMocks() {
  arduino_mock::reset();
  Wire.reset();
  tinygps_mock::reset();
  geometry_msgs_mock::reset();
  rosidl_mock::reset();
}
