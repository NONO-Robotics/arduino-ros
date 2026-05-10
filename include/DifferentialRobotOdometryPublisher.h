#pragma once
#include "DifferentialRobotOdometry.h"
#include "Vector3StampedPublisher.h"
#include "MicroRosPublisher.h"

/**
 * @brief Publisher for differential robot odometry data.
 *
 * This class wraps a Vector3StampedPublisher to publish odometry
 * data (x, y, theta) calculated from a differential drive robot.
 */
class DifferentialRobotOdometryPublisher {
private:
  Vector3StampedPublisher *publisher;

public:
  /**
   * @brief Constructor for DifferentialRobotOdometryPublisher.
   *
   * @param node Pointer to the ROS node.
   * @param topic_name Name of the topic to publish to (default: "odometry").
   * @param frameId Frame ID for the odometry data (default: "base_link").
   */
  DifferentialRobotOdometryPublisher(rcl_node_t *node,
                                     String topic_name = "odometry",
                                     String frameId = "base_link");

  /**
   * @brief Destructor for DifferentialRobotOdometryPublisher.
   */
  ~DifferentialRobotOdometryPublisher();

  /**
   * @brief Publishes the odometry data.
   *
   * @param data Reference to the DifferentialRobotOdometry data object.
   */
  void publish(const DifferentialRobotOdometry &data);
};
