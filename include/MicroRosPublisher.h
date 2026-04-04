#pragma once
#include "RosMessage.h"
#include "RosUtils.h"
#include <StringUtils.h>

/**
 * MicroRosPublisher class for publishing messages using micro-ROS.
 *
 * This class provides methods to create a publisher and publish messages
 * using the micro-ROS framework.
 */
class MicroRosPublisher
{
private:
  rcl_publisher_t publisher;
  int delayMillis;

public:
  /**
   * Constructor for MicroRosPublisher.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param type_support Type support for the message
   * @param delayMillis Delay in milliseconds between messages
   */
  static MicroRosPublisher *createInt(rcl_node_t *node, String topic_name,
                                      int delayMillis = 0,
                                      bool reliable = true);

  /**
   * Create a publisher for float arrays.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param delayMillis Delay in milliseconds between messages
   */
  static MicroRosPublisher *createFloat(rcl_node_t *node, String topic_name,
                                        int delayMillis = 0,
                                        bool reliable = true);

  /**
   * Create a publisher for float arrays.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param delayMillis Delay in milliseconds between messages
   */
  static MicroRosPublisher *createFloatArray(rcl_node_t *node,
                                             String topic_name,
                                             int delayMillis = 0,
                                             bool reliable = true);

  static MicroRosPublisher *createVector3Stamped(rcl_node_t *node,
                                                 String topic_name,
                                                 int delayMillis = 0,
                                                 bool reliable = true);

  /**
   * @brief Create a publisher for string messages.
   * @param node Pointer to the ROS node.
   * @param topic_name Name of the topic to publish to.
   * @param delayMillis (Optional) Delay in milliseconds between messages.
   * Default is 0.
   * @param reliable (Optional) Whether to use reliable QoS. Default is true.
   * @return Pointer to the created MicroRosPublisher instance.
   */
  static MicroRosPublisher *createString(rcl_node_t *node, String topic_name,
                                         int delayMillis = 0,
                                         bool reliable = true);

  /**
   * @brief Constructor for MicroRosPublisher.
   *
   * @param node Pointer to the ROS node.
   * @param topic_name Name of the topic to publish to.
   * @param type_support Type support for the message.
   * @param delayMillis (Optional) Delay in milliseconds between messages.
   * Default is 0.
   * @param reliable (Optional) True for reliable QoS, false for best effort.
   * Default is true.
   */
  MicroRosPublisher(rcl_node_t *node, String topic_name,
                    const rosidl_message_type_support_t *type_support,
                    int delayMillis = 0, bool reliable = true);

  /**
   * @brief Publish a message.
   *
   * Sends the provided message to the configured topic.
   *
   * @param message Pointer to the message structure to be published.
   */
  void publish(const void *message);
};