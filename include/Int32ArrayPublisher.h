#pragma once

#include "MicroRosPublisher.h"
#include "RosMessage.h"

/** @brief Publishes fixed-length Int32 arrays through micro-ROS. */
class Int32ArrayPublisher {
public:
  /**
   * @brief Create an Int32 array publisher with a persistent ROS message.
   * @param publisher Caller-owned micro-ROS publisher transferred to this class.
   * @param length Number of values in every published array.
   */
  Int32ArrayPublisher(MicroRosPublisher *publisher, size_t length);

  /** @brief Destroy the owned ROS message and publisher. */
  ~Int32ArrayPublisher();

  /**
   * @brief Copy and publish an Int32 array.
   * @param dataArray Values to copy into the ROS message.
   */
  void publish(const int32_t *dataArray);

private:
  MicroRosPublisher *publisher;
  std_msgs__msg__Int32MultiArray *msg;
  size_t length;
};
