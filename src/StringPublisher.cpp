#include "StringPublisher.h"

StringPublisher::StringPublisher(MicroRosPublisher *publisher) : publisher(publisher)
{
    // Allocate memory and initialize String message
    msg = std_msgs__msg__String__create();
    if (msg)
    {
        // You can pre-allocate capacity if you know an approximate max size
        // or let it reallocate dynamically with __assign.
        // For example, to pre-allocate for 50 chars:
        // rosidl_runtime_c__String__init(&msg->data);
        // if (!rosidl_runtime_c__String__assignn(&msg->data, "", 0)) {
        //    // Handle allocation error if needed
        // }
        // Or simply initialize empty:
        rosidl_runtime_c__String__init(&msg->data);
    }
    else
    {
        // Handle message creation error if needed
    }
}

StringPublisher::~StringPublisher()
{
    if (msg)
    {
        std_msgs__msg__String__destroy(msg);
        msg = nullptr;
    }
}

void StringPublisher::publish(const char *data_str)
{
    if (publisher && msg)
    {
        if (rosidl_runtime_c__String__assign(&msg->data, data_str))
        {
            publisher->publish(msg);
        }
        else
        {
            // Handle string assignment error if needed
            // For example, print an error via Serial
        }
    }
}

void StringPublisher::publish(const String &data_str) { publish(data_str.c_str()); }
