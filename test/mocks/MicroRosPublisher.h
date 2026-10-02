#pragma once

struct rcl_node_t;

class MicroRosPublisher {
 public:
  static MicroRosPublisher* createString(rcl_node_t*, const String&) {
    return nullptr;
  }
};
