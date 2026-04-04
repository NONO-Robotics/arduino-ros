#include "MicroRosPublisher.h"

MicroRosPublisher *MicroRosPublisher::createInt(
    rcl_node_t *node,
    String topic_name,
    int delayMillis,
    bool reliable)
{
  return new MicroRosPublisher(
      node,
      topic_name,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      delayMillis,
      reliable);
};

MicroRosPublisher *MicroRosPublisher::createFloat(
    rcl_node_t *node,
    String topic_name,
    int delayMillis,
    bool reliable)
{
  return new MicroRosPublisher(
      node,
      topic_name,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
      delayMillis,
      reliable);
};

MicroRosPublisher *MicroRosPublisher::createFloatArray(
    rcl_node_t *node,
    String topic_name,
    int delayMillis,
    bool reliable)
{
  return new MicroRosPublisher(
      node,
      topic_name,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
      delayMillis,
      reliable);
};

MicroRosPublisher *MicroRosPublisher::createVector3Stamped(
    rcl_node_t *node,
    String topic_name,
    int delayMillis,
    bool reliable)
{
  return new MicroRosPublisher(
      node,
      topic_name,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Vector3Stamped),
      delayMillis,
      reliable);
}

MicroRosPublisher *MicroRosPublisher::createString(
    rcl_node_t *node,
    String topic_name,
    int delayMillis,
    bool reliable)
{
  return new MicroRosPublisher(
      node,
      topic_name,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
      delayMillis,
      reliable);
}

MicroRosPublisher::MicroRosPublisher(
    rcl_node_t *node,
    String topic_name,
    const rosidl_message_type_support_t *type_support,
    int delayMillis,
    bool reliable)
{
  if (reliable)
  {
    assertOk(
        rclc_publisher_init_default(
            &this->publisher, node, type_support, toCharArray(topic_name)),
        "Error to create reliable publisher");
  }
  else
  {
    assertOk(
        rclc_publisher_init_best_effort(
            &this->publisher, node, type_support, toCharArray(topic_name)),
        "Error to create best_effort publisher");
  }
  this->delayMillis = delayMillis;
};

void MicroRosPublisher::publish(const void *message)
{
  assertOk(
      rcl_publish(
          &this->publisher,
          message,
          NULL),
      "Error to publish message");
  delay(this->delayMillis);
};
