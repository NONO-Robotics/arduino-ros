#pragma once

#include <Arduino.h>

class MicroRosPublisher;

class StringPublisher {
 public:
  explicit StringPublisher(MicroRosPublisher*) {}
  void publish(const String&) {}
};
