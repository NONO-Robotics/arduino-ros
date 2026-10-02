#include "WheelSpeedsSubscriber.h"

#include <cmath>

#include "RosUtils.h"

WheelSpeedsSubscriber *WheelSpeedsSubscriber::instance_ = nullptr;

WheelSpeedsSubscriber::WheelSpeedsSubscriber(
    rcl_node_t *node, rclc_executor_t *executor, const char *topic,
    WheelSpeedEvent onWheelSpeed, FaultEvent onFault)
    : onWheelSpeed_(onWheelSpeed), onFault_(onFault)
{
  const String topicName(topic);

  message_.data.data = rawWheelSpeeds_;
  message_.data.capacity = WheelSpeedCount;
  message_.data.size = 0;
  instance_ = this;

  assertOk(
      rclc_subscription_init_default(
          &subscriber_, node,
          ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
          topic),
      "Error to create subscriber: " + topicName);

  assertOk(
      rclc_executor_add_subscription(executor, &subscriber_, &message_,
                                     onMessage, ON_NEW_DATA),
      "Error suscribing to topic: " + topicName +
          " with std_msgs::msg::Float32MultiArray type");
}

void WheelSpeedsSubscriber::onMessage(const void *message)
{
  if (instance_ != nullptr)
  {
    instance_->handleMessage(message);
  }
}

bool WheelSpeedsSubscriber::isValid(
    const std_msgs__msg__Float32MultiArray *wheelSpeeds)
{
  return wheelSpeeds != nullptr && wheelSpeeds->data.data != nullptr &&
         wheelSpeeds->data.size == WheelSpeedCount &&
         std::isfinite(wheelSpeeds->data.data[0]) &&
         std::isfinite(wheelSpeeds->data.data[1]) &&
         std::isfinite(wheelSpeeds->data.data[2]) &&
         std::isfinite(wheelSpeeds->data.data[3]) &&
         std::isfinite(wheelSpeeds->data.data[4]) &&
         std::isfinite(wheelSpeeds->data.data[5]);
}

void WheelSpeedsSubscriber::handleMessage(const void *message)
{
  const auto *wheelSpeeds =
      static_cast<const std_msgs__msg__Float32MultiArray *>(message);
  if (!isValid(wheelSpeeds))
  {
    if (onFault_ != nullptr)
    {
      onFault_();
    }
    return;
  }

  wheelSpeeds_ = WheelSpeeds(
      wheelSpeeds->data.data[0], wheelSpeeds->data.data[1],
      wheelSpeeds->data.data[2], wheelSpeeds->data.data[3],
      wheelSpeeds->data.data[4], wheelSpeeds->data.data[5]);
  if (onWheelSpeed_ != nullptr)
  {
    onWheelSpeed_(wheelSpeeds_);
  }
}
