#include "FloatPublisher.h"

FloatPublisher::FloatPublisher(MicroRosPublisher *publisher) {
  this->publisher = publisher;
  msg = createFloatMessage();
}

void FloatPublisher::publish(int value) {
  msg->data = value;
  publisher->publish(msg);
}