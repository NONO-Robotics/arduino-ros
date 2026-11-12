#pragma once

#include <cstddef>
#include <cstdint>

struct std_msgs__msg__Int32__Sequence {
  int32_t *data = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};

struct std_msgs__msg__Int32MultiArray {
  std_msgs__msg__Int32__Sequence data;
};
