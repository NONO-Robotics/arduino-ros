#pragma once

#include <geometry_msgs/msg/vector3_stamped.h>

namespace time_mock {
inline int stamps = 0;
inline void reset() { stamps = 0; }
}  // namespace time_mock

class MicroRosTimeUtils {
 public:
  static bool syncSession(int timeout_ms = 500) {
    (void)timeout_ms;
    return true;
  }
  static bool syncSessionWithRetry(int timeout_ms = 500, int attempts = 50) {
    (void)timeout_ms;
    (void)attempts;
    return true;
  }
  static bool isSynchronized() { return true; }
  static void setCurrentStamp(std_msgs__msg__Header *header) {
    ++time_mock::stamps;
    if (header) header->stamp.sec = time_mock::stamps;
  }
};
