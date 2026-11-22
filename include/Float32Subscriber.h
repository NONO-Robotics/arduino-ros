#pragma once

#include <RosUtils.h>
#include <rclc/executor.h>
#include <std_msgs/msg/float32.h>

/** @brief Callback invoked when a Float32 message arrives. */
using Float32Event = void (*)(const std_msgs__msg__Float32 &message);

/**
 * @brief Subscriber for Float32 ROS 2 messages.
 *
 * Owns a Float32 message buffer and dispatches received data to a typed
 * callback.
 */
class Float32Subscriber {
 private:
  rcl_subscription_t subscriber_{};
  std_msgs__msg__Float32 message_{};
  Float32Event event_;
  Float32Subscriber *next_;

  static Float32Subscriber *first_;
  static void onExecutorMessage(const void *message);

 public:
  /**
   * @brief Creates and registers a Float32 subscription.
   *
   * @param node ROS node that owns this subscription.
   * @param executor Executor that dispatches received messages.
   * @param topic Topic name.
   * @param onReceiveMessage Typed callback invoked for each received message.
   */
  Float32Subscriber(rcl_node_t *node, rclc_executor_t *executor,
                    const char *topic, Float32Event onReceiveMessage);
};
