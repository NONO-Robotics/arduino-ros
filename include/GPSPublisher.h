#pragma once

#include <rcl/publisher.h>
#include <rcl/node.h>
#include <micro_ros_platformio.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <sensor_msgs/msg/nav_sat_fix.h>
#include <geometry_msgs/msg/vector3.h>

#include "GPSData.h"
#include "Logger.h"
#include "StringUtils.h"

class GPSPublisher
{
private:
    void prepareMsg();

    rcl_publisher_t publisher;
    sensor_msgs__msg__NavSatFix msg;
    rcl_node_t *node_ptr; // Store pointer for cleanup
    String frameId;

public:
    // Constructor: Initializes the publisher.
    GPSPublisher(
        rcl_node_t *node,
        rcl_allocator_t *allocator,
        String topic_name = "/gps/fix",
        String frameId = "gps_link");

    void publish(GPSData *data);
};