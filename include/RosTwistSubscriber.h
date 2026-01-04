#pragma once
#include "RosUtils.h"
#include <StringUtils.h>
#include <geometry_msgs/msg/twist.h>
#include <rclc/executor.h>

/**
 * @brief Class for subscribing to Twist messages using micro-ROS.
 *
 * This class provides methods to create a subscriber and handle incoming
 * Twist messages using the micro-ROS framework.
 */
class RosTwistSubscriber {

private:
  rcl_subscription_t subscriber;
  geometry_msgs__msg__Twist msg;

public:
  /**
   * @brief Constructor for RosTwistSubscriber class.
   * @param node Pointer to the ROS node.
   * @param executor Pointer to the ROS executor.
   * @param name Name of the topic to subscribe to.
   * @param onReceiveMessage Callback function to handle incoming messages.
   *                         The callback receives a void pointer to the message
   * (geometry_msgs__msg__Twist*).
   */
  RosTwistSubscriber(rcl_node_t *node, rclc_executor_t *executor, String name,
                     void (*onReceiveMessage)(const void *));

  /**
   * @brief Get twist message.
   */
  geometry_msgs__msg__Twist *getTwist();
};