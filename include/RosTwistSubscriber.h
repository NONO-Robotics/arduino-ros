#pragma once
#include "RosUtils.h"
#include <StringUtils.h>
#include <geometry_msgs/msg/twist.h>
#include <rclc/executor.h>

/** @brief Callback invoked when a Twist message arrives. */
using TwistEvent = void (*)(geometry_msgs__msg__Twist *twist);

/**
 * @brief Subscriber for velocity commands (`geometry_msgs/msg/Twist`).
 *
 * This class abstracts the subscription to a ROS 2 Twist topic (usually `cmd_vel`).
 * 
 * Usage Context: Primarily used in the robot's movement nodes to receive
 * teleoperation commands or autonomous navigation (Nav2) velocity commands,
 * which are then translated into wheel speeds by the traction controllers.
 */
class RosTwistSubscriber {

private:
  rcl_subscription_t subscriber;
  geometry_msgs__msg__Twist msg;
  TwistEvent event;
  RosTwistSubscriber *next;

  static RosTwistSubscriber *first;
  static void onExecutorMessage(const void *message);

public:
  /**
   * @brief Constructor for RosTwistSubscriber class.
   * @param node Pointer to the ROS node.
   * @param executor Pointer to the ROS executor.
   * @param name Name of the topic to subscribe to.
   * @param onReceiveMessage Typed callback invoked with the received Twist.
   */
  RosTwistSubscriber(rcl_node_t *node, rclc_executor_t *executor, String name,
                     TwistEvent onReceiveMessage);
};
