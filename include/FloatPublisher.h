#pragma once
#include "MicroRosPublisher.h"
#include "RosMessage.h"

/**
 * FloatPublisher class for publishing float values using micro-ROS.
 *
 * This class provides methods to create a publisher and publish float values
 * using the micro-ROS framework.
 */
class FloatPublisher {
public:
  /**
   * @brief Constructor for FloatPublisher.
   * @param publisher Pointer to the MicroRosPublisher object.
   */
  FloatPublisher(MicroRosPublisher *publisher);

  /**
   * @brief Publish the float value.
   * @param value Float value to be published.
   */
  void publish(float value);

private:
  MicroRosPublisher *publisher;
  std_msgs__msg__Float32 *msg;
};