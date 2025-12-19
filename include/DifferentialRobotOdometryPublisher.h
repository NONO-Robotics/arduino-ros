#pragma once
#include "DifferentialRobotOdometry.h"
#include "MicroRosPublisher.h"
#include "FloatArrayPublisher.h"

/**
 * DifferentialRobotOdometryPublisher class for publishing odometry data.
 *
 * This class provides methods to publish odometry data and transform messages
 * using the ROS 2 micro-ROS framework.
 */
class DifferentialRobotOdometryPublisher
{
private:
    FloatArrayPublisher *publisher;

public:
    /**
     * Constructor for DifferentialRobotOdometryPublisher.
     * @param node Pointer to the ROS node
     */
    DifferentialRobotOdometryPublisher(rcl_node_t *node, String topic_name = "odometry");
    
    ~DifferentialRobotOdometryPublisher();

    /**
     * Publish the odometry data.
     * @param data Robot movement data.
     */
    void publish(const DifferentialRobotOdometry &data);
};