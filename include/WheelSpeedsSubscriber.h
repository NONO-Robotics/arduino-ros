#pragma once

#include <cstddef>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/float32_multi_array.h>

#include <WheelSpeeds.h>

/**
 * @brief Receives, validates, and dispatches wheel-speed events.
 *
 * Owns its ROS subscription, fixed-size message storage, and message
 * validation. It translates six transport values into typed wheel speeds and
 * emits either a valid speed update or a fault event.
 */
class WheelSpeedsSubscriber {
 private:
  static constexpr std::size_t WheelSpeedCount = 6;

  /** @brief Invoked when received wheel speeds pass validation. */
  using WheelSpeedEvent = void (*)(const WheelSpeeds &wheelSpeeds);

  /** @brief Invoked when received wheel speeds are invalid. */
  using FaultEvent = void (*)();

  rcl_subscription_t subscriber_{};
  std_msgs__msg__Float32MultiArray message_{};
  float rawWheelSpeeds_[WheelSpeedCount]{};
  WheelSpeeds wheelSpeeds_{0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
  WheelSpeedEvent onWheelSpeed_;
  FaultEvent onFault_;

  static WheelSpeedsSubscriber *instance_;

  static void onMessage(const void *message);
  static bool isValid(const std_msgs__msg__Float32MultiArray *wheelSpeeds);
  void handleMessage(const void *message);

 public:
  /**
   * @brief Creates and registers a wheel-speed subscription.
   *
   * @param node ROS node that owns this subscription.
   * @param executor Executor that dispatches received wheel speeds.
   * @param topic Wheel-speed topic name.
   * @param onWheelSpeed Callback invoked for each valid wheel-speed message.
   * @param onFault Callback invoked for each invalid wheel-speed message.
   */
  WheelSpeedsSubscriber(rcl_node_t *node, rclc_executor_t *executor,
                        const char *topic, WheelSpeedEvent onWheelSpeed,
                        FaultEvent onFault);
};
