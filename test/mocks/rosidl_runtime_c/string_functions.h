#pragma once

#include <cstdlib>
#include <cstring>

#include <geometry_msgs/msg/vector3_stamped.h>

namespace rosidl_mock {
inline bool assign_fails = false;

inline void reset() { assign_fails = false; }
}  // namespace rosidl_mock

inline bool rosidl_runtime_c__String__assign(rosidl_runtime_c__String* value,
                                             const char* text) {
  if (rosidl_mock::assign_fails) return false;
  size_t length = std::strlen(text);
  value->data = static_cast<char*>(std::malloc(length + 1));
  if (value->data == nullptr) return false;
  std::memcpy(value->data, text, length + 1);
  value->size = length;
  value->capacity = length + 1;
  return true;
}

inline bool rosidl_runtime_c__String__init(rosidl_runtime_c__String* value) {
  return rosidl_runtime_c__String__assign(value, "");
}
