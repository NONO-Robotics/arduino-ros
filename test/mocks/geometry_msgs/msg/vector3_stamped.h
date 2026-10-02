#pragma once

#include <cstdlib>

#include <geometry_msgs/msg/twist.h>

struct rosidl_runtime_c__String {
  char* data = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};

struct builtin_interfaces__msg__Time {
  int32_t sec = 0;
  uint32_t nanosec = 0;
};

struct std_msgs__msg__Header {
  builtin_interfaces__msg__Time stamp;
  rosidl_runtime_c__String frame_id;
};

struct geometry_msgs__msg__Vector3Stamped {
  std_msgs__msg__Header header;
  geometry_msgs__msg__Vector3 vector;
};

namespace geometry_msgs_mock {
inline bool create_fails = false;
inline int creates = 0;
inline int destroys = 0;

inline void reset() {
  create_fails = false;
  creates = 0;
  destroys = 0;
}
}  // namespace geometry_msgs_mock

inline geometry_msgs__msg__Vector3Stamped*
geometry_msgs__msg__Vector3Stamped__create() {
  ++geometry_msgs_mock::creates;
  return geometry_msgs_mock::create_fails
             ? nullptr
             : new geometry_msgs__msg__Vector3Stamped();
}

inline void geometry_msgs__msg__Vector3Stamped__destroy(
    geometry_msgs__msg__Vector3Stamped* message) {
  ++geometry_msgs_mock::destroys;
  std::free(message->header.frame_id.data);
  delete message;
}
