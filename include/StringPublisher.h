#pragma once
#include "MicroRosPublisher.h"
#include "RosMessage.h"

/**
 * StringPublisher class for publishing string messages using micro-ROS.
 *
 * This class provides methods to create a publisher and publish string messages
 * using the micro-ROS framework.
 */
class StringPublisher {
public:
  /**
   * Constructor for StringPublisher.
   * @param publisher Pointer to the MicroRosPublisher object.
   */
  StringPublisher(MicroRosPublisher *publisher) : publisher(publisher) {
    // Allocate memory and initialize String message
    msg = std_msgs__msg__String__create();
    if (msg) {
      // You can pre-allocate capacity if you know an approximate max size
      // or let it reallocate dynamically with __assign.
      // For example, to pre-allocate for 50 chars:
      // rosidl_runtime_c__String__init(&msg->data);
      // if (!rosidl_runtime_c__String__assignn(&msg->data, "", 0)) {
      //    // Handle allocation error if needed
      // }
      // Or simply initialize empty:
      rosidl_runtime_c__String__init(&msg->data);
    } else {
      // Handle message creation error if needed
    }
  }

  /**
   * Destructor for StringPublisher.
   * Frees message memory.
   */
  ~StringPublisher() {
    if (msg) {
      std_msgs__msg__String__destroy(msg);
      msg = nullptr;
    }
  }

  /**
   * Publish the string message.
   * @param data_str Pointer to the C-style string (char array) to be published.
   */
  void publish(const char *data_str) {
    if (publisher && msg) {
      if (rosidl_runtime_c__String__assign(&msg->data, data_str)) {
        publisher->publish(msg);
      } else {
        // Handle string assignment error if needed
        // For example, print an error via Serial
      }
    }
  }

  /**
   * Publish the string message.
   * @param data_str Arduino String object to be published.
   */
  void publish(const String &data_str) { publish(data_str.c_str()); }

private:
  MicroRosPublisher *publisher;
  std_msgs__msg__String *msg;
};
