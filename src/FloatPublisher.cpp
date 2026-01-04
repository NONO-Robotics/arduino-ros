#include "FloatPublisher.h"

FloatPublisher::FloatPublisher(MicroRosPublisher *publisher) {
  this->publisher = publisher;
  msg = createFloatMessage();
}

void FloatPublisher::publish(float value) {
  msg->data = value;
  publisher->publish(msg);
}