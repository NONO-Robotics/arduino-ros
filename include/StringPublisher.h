#pragma once
#include "MicroRosPublisher.h"
#include "RosMessage.h"

/**
 * @brief StringPublisher class for publishing string messages using micro-ROS.
 *
 * This class provides methods to create a publisher and publish string messages
 * using the micro-ROS framework.
 */
class StringPublisher {
private:
  MicroRosPublisher *publisher;
  std_msgs__msg__String *msg;

public:
  /**
   * @brief Constructor for StringPublisher.
   * @param publisher Pointer to the MicroRosPublisher object.
   */
  StringPublisher(MicroRosPublisher *publisher);

  /**
   * @brief Destructor for StringPublisher.
   * Frees message memory.
   */
  ~StringPublisher();

  /**
   * @brief Publish the string message.
   * @param data_str Pointer to the C-style string (char array) to be published.
   */
  void publish(const char *data_str);

  /**
   * @brief Publish the string message.
   * @param data_str Arduino String object to be published.
   */
  void publish(const String &data_str);
};
