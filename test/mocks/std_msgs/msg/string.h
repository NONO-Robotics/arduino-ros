#pragma once

#include <geometry_msgs/msg/vector3_stamped.h>
#include <rosidl_runtime_c/string_functions.h>

struct std_msgs__msg__String {
  rosidl_runtime_c__String data;
};

namespace std_msgs_string_mock {
inline bool create_fails = false;
inline int creates = 0;
inline int destroys = 0;

inline void reset() {
  create_fails = false;
  creates = 0;
  destroys = 0;
}
}  // namespace std_msgs_string_mock

inline std_msgs__msg__String* std_msgs__msg__String__create() {
  ++std_msgs_string_mock::creates;
  if (std_msgs_string_mock::create_fails) return nullptr;
  auto* message = new std_msgs__msg__String();
  message->data = rosidl_runtime_c__String{};
  return message;
}

inline void std_msgs__msg__String__destroy(std_msgs__msg__String* message) {
  ++std_msgs_string_mock::destroys;
  delete message;
}
