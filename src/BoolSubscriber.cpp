#include "BoolSubscriber.h"

BoolSubscriber *BoolSubscriber::first_ = nullptr;

BoolSubscriber::BoolSubscriber(rcl_node_t *node, rclc_executor_t *executor,
                               const char *topic, BoolEvent onReceiveMessage)
    : event_(onReceiveMessage), next_(first_)
{
  const String topicName(topic);
  first_ = this;

  assertOk(
      rclc_subscription_init_default(
          &subscriber_, node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
          topic),
      "Error to create subscriber: " + topicName);

  assertOk(rclc_executor_add_subscription(executor, &subscriber_, &message_,
                                          onExecutorMessage, ON_NEW_DATA),
           "Error suscribing to topic: " + topicName +
               " with std_msgs::msg::Bool type");
}

void BoolSubscriber::onExecutorMessage(const void *message)
{
  if (message == nullptr)
  {
    return;
  }

  for (BoolSubscriber *subscriber = first_; subscriber != nullptr;
       subscriber = subscriber->next_)
  {
    if (message == &subscriber->message_ && subscriber->event_ != nullptr)
    {
      subscriber->event_(subscriber->message_);
      return;
    }
  }
}
