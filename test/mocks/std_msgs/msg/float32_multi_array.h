#pragma once

#include <cstddef>

struct std_msgs__msg__Float__Sequence {
  float* data = nullptr;
  size_t size = 0;
  size_t capacity = 0;
};

struct std_msgs__msg__Float32MultiArray {
  std_msgs__msg__Float__Sequence data;
};
