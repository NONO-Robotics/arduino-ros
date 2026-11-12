#include "Int32ArrayPublisher.h"

Int32ArrayPublisher::Int32ArrayPublisher(MicroRosPublisher *publisher,
                                         size_t length)
    : publisher(publisher), msg(createInt32ArrayMessage(length)), length(length) {}

void Int32ArrayPublisher::publish(const int32_t *dataArray) {
  for (size_t index = 0; index < length; ++index) {
    msg->data.data[index] = dataArray[index];
  }
  publisher->publish(msg);
}

Int32ArrayPublisher::~Int32ArrayPublisher() {
  if (msg) {
    free(msg->data.data);
    delete msg;
  }
  delete publisher;
}
