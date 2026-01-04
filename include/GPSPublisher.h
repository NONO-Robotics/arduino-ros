#pragma once

#include <micro_ros_platformio.h>
#include <rcl/node.h>
#include <rcl/publisher.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>

#include "GPSData.h"
#include "Logger.h"
#include "NavSatFixMsgWriter.h"
#include "StringUtils.h"

/**
 * @brief GPSPublisher class for publishing GPS data.
 *
 * This class handles the initialization and publishing of GPS data (NavSatFix)
 * to a ROS 2 topic using micro-ROS.
 */
class GPSPublisher {
private:
  void prepareMsg();

  rcl_publisher_t publisher;
  sensor_msgs__msg__NavSatFix msg;
  rcl_node_t *node_ptr; // Store pointer for cleanup
  String frameId;
  NavSatFixMsgWriter *msgWriter;

public:
  /**
   * @brief Constructor for GPSPublisher.
   *
   * @param node Pointer to the ROS node.
   * @param allocator Pointer to the micro-ROS allocator.
   * @param topic_name (Optional) Name of the topic to publish to. Default is
   * "/gps/fix".
   * @param frameId (Optional) Frame ID for the GPS data. Default is "gps_link".
   */
  GPSPublisher(rcl_node_t *node, rcl_allocator_t *allocator,
               String topic_name = "/gps/fix", String frameId = "gps_link");

  /**
   * @brief Publishes the GPS data.
   *
   * @param data Pointer to the GPSData object containing the data to publish.
   */
  void publish(GPSData *data);
};