#pragma once

#include <micro_ros_platformio.h>
#include <rcl/node.h>
#include <rcl/publisher.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <sensor_msgs/msg/imu.h>

#include "IMUData.h"
#include "IMUMsgWriter.h"
#include "Logger.h"
#include "StringUtils.h"

/**
 * @brief IMUPublisher class for publishing IMU data.
 *
 * This class handles the initialization and publishing of IMU data
 * (accelerometer, gyroscope, orientation) to a ROS 2 topic using micro-ROS.
 */
class IMUPublisher {
private:
  void prepareMsg();

  rcl_publisher_t publisher;
  sensor_msgs__msg__Imu msg;
  rcl_node_t *node_ptr; // Store pointer for cleanup
  String frameId;
  IMUMsgWriter *msgWriter;

public:
  /**
   * @brief Constructor for IMUPublisher.
   *
   * @param node Pointer to the ROS node.
   * @param allocator Pointer to the micro-ROS allocator.
   * @param topic_name (Optional) Name of the topic to publish to. Default is
   * "/imu/data".
   * @param frameId (Optional) Frame ID for the IMU data. Default is "imu_link".
   */
  IMUPublisher(rcl_node_t *node, rcl_allocator_t *allocator,
               String topic_name = "/imu/data", String frameId = "imu_link");

  /**
   * @brief Publishes the IMU data.
   *
   * @param data Pointer to the IMUData object containing the data to publish.
   */
  void publish(IMUData *data);
};