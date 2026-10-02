#pragma once

#include <cstdint>

constexpr int8_t sensor_msgs__msg__NavSatStatus__STATUS_NO_FIX = -1;
constexpr int8_t sensor_msgs__msg__NavSatStatus__STATUS_FIX = 0;
constexpr uint16_t sensor_msgs__msg__NavSatStatus__SERVICE_GPS = 1;
constexpr uint8_t sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_UNKNOWN = 0;
constexpr uint8_t sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_DIAGONAL_KNOWN = 2;

struct sensor_msgs__msg__NavSatStatus {
  int8_t status = 0;
  uint16_t service = 0;
};

struct sensor_msgs__msg__NavSatFix {
  sensor_msgs__msg__NavSatStatus status;
  double latitude = 0;
  double longitude = 0;
  double altitude = 0;
  double position_covariance[9] = {};
  uint8_t position_covariance_type = 0;
};
