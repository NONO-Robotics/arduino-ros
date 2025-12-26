#pragma once

#include <rcl/publisher.h>
#include <rcl/node.h>
#include <sensor_msgs/msg/imu.h>
#include <micro_ros_platformio.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include "IMUData.h"
#include "Logger.h"
#include "StringUtils.h"
#include "IMUMsgWriter.h"

class IMUPublisher
{
private:
    void prepareMsg();

    rcl_publisher_t publisher;
    sensor_msgs__msg__Imu msg;
    rcl_node_t *node_ptr; // Store pointer for cleanup
    String frameId;
    IMUMsgWriter *msgWriter;

public:
    // Constructor: Initializes the publisher.
    IMUPublisher(
        rcl_node_t *node,
        rcl_allocator_t *allocator,
        String topic_name = "/imu/data",
        String frameId = "imu_link");

    // Publishes the IMU data.
    void publish(IMUData *data);
};