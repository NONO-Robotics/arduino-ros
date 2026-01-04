#pragma once
#include "DifferentialRobotOdometry.h"
#include "FloatArrayPublisher.h"
#include "MicroRosPublisher.h"

/**
 * DifferentialRobotOdometryPublisher class for publishing odometry data.
 *
 * This class provides methods to publish odometry data and transform messages
 * using the ROS 2 micro-ROS framework.
 */
class DifferentialRobotOdometryPublisher {
private:
  FloatArrayPublisher *publisher;

public:
  /**
   * @brief Constructor for DifferentialRobotOdometryPublisher.
   * @param node Pointer to the ROS node.
   * @param topic_name (Optional) Name of the topic to publish to. Default is
   * "odometry".
   */
  DifferentialRobotOdometryPublisher(rcl_node_t *node,
                                     String topic_name = "odometry");

  ~DifferentialRobotOdometryPublisher();

  /**
   * @brief Publish the odometry data.
   * @param data Robot movement data (odometry) to be published.
   */
  void publish(const DifferentialRobotOdometry &data);
};