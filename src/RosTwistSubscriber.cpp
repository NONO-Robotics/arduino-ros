
#include "RosTwistSubscriber.h"

RosTwistSubscriber *RosTwistSubscriber::first = nullptr;

RosTwistSubscriber::RosTwistSubscriber(
    rcl_node_t *node,
    rclc_executor_t *executor,
    String name,
    TwistEvent onReceiveMessage)
    : event(onReceiveMessage), next(first)
{
    msg = geometry_msgs__msg__Twist();
    first = this;

    // Create subscriber
    assertOk(
        rclc_subscription_init_default(
            &subscriber,
            node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
            toCharArray(name)),
        "Error to create subscriber: " + name);

    assertOk(
        rclc_executor_add_subscription(
            executor,
            &subscriber,
            &msg,
            onExecutorMessage,
            ON_NEW_DATA),
        "Error suscribing to topic: " + name + " with geometry_msgs::msg::Twist type");
}

void RosTwistSubscriber::onExecutorMessage(const void *message)
{
    if (message == nullptr)
    {
        return;
    }

    for (RosTwistSubscriber *subscriber = first;
         subscriber != nullptr;
         subscriber = subscriber->next)
    {
        if (message == &subscriber->msg && subscriber->event != nullptr)
        {
            subscriber->event(&subscriber->msg);
            return;
        }
    }
}
