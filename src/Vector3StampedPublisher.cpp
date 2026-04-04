#include "Vector3StampedPublisher.h"
#include "MicroRosTimeUtils.h"
#include <stdlib.h>

Vector3StampedPublisher::Vector3StampedPublisher(
    MicroRosPublisher *publisher,
    String frameId
) : publisher(publisher) {
    this->msg = createVector3StampedMessage(frameId);
}

Vector3StampedPublisher::~Vector3StampedPublisher() {
    if (msg) {
        geometry_msgs__msg__Vector3Stamped__destroy(msg);
        msg = nullptr;
    }
    if (publisher) {
        delete publisher;
        publisher = nullptr;
    }
}

void Vector3StampedPublisher::publish(float x, float y, float z) {
    if (!msg || !publisher) return;

    MicroRosTimeUtils::setCurrentStamp(&msg->header);
    
    msg->vector.x = x;
    msg->vector.y = y;
    msg->vector.z = z;

    publisher->publish(msg);
}
