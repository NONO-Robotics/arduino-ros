#pragma once
#include "DifferentialRobotOdometry.h"
#include "Vector3StampedPublisher.h"
#include "MicroRosPublisher.h"

class DifferentialRobotOdometryPublisher {
private:
  Vector3StampedPublisher *publisher;

public:
  DifferentialRobotOdometryPublisher(rcl_node_t *node,
                                     String topic_name = "odometry",
                                     String frameId = "base_link");
  ~DifferentialRobotOdometryPublisher();
  void publish(const DifferentialRobotOdometry &data);
};
