#pragma once

#include <RosUtils.h>
#include <rclc/executor.h>
#include <std_msgs/msg/bool.h>

/** @brief Callback invoked when a Boolean message arrives. */
using BoolEvent = void (*)(const std_msgs__msg__Bool &message);

/**
 * @brief Subscriber for Boolean ROS 2 messages.
 *
 * Owns a Boolean message buffer and dispatches received data to a typed
 * callback.
 */
class BoolSubscriber {
 private:
  rcl_subscription_t subscriber_{};
  std_msgs__msg__Bool message_{};
  BoolEvent event_;
  BoolSubscriber *next_;

  static BoolSubscriber *first_;
  static void onExecutorMessage(const void *message);

 public:
  /**
   * @brief Creates and registers a Boolean subscription.
   *
   * @param node ROS node that owns this subscription.
   * @param executor Executor that dispatches received messages.
   * @param topic Topic name.
   * @param onReceiveMessage Typed callback invoked for each received message.
   */
  BoolSubscriber(rcl_node_t *node, rclc_executor_t *executor,
                 const char *topic, BoolEvent onReceiveMessage);
};
