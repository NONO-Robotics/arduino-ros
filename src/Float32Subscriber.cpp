#include "Float32Subscriber.h"

Float32Subscriber *Float32Subscriber::first_ = nullptr;

Float32Subscriber::Float32Subscriber(rcl_node_t *node, rclc_executor_t *executor,
                                     const char *topic,
                                     Float32Event onReceiveMessage)
    : event_(onReceiveMessage), next_(first_)
{
  const String topicName(topic);
  first_ = this;

  assertOk(
      rclc_subscription_init_default(
          &subscriber_, node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
          topic),
      "Error to create subscriber: " + topicName);

  assertOk(rclc_executor_add_subscription(executor, &subscriber_, &message_,
                                          onExecutorMessage, ON_NEW_DATA),
           "Error subscribing to topic: " + topicName +
               " with std_msgs::msg::Float32 type");
}

void Float32Subscriber::onExecutorMessage(const void *message)
{
  if (message == nullptr)
  {
    return;
  }

  for (Float32Subscriber *subscriber = first_; subscriber != nullptr;
       subscriber = subscriber->next_)
  {
    if (message == &subscriber->message_ && subscriber->event_ != nullptr)
    {
      subscriber->event_(subscriber->message_);
      return;
    }
  }
}
