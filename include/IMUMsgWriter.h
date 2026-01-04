#pragma once
#include "IMUData.h"
#include <sensor_msgs/msg/imu.h>

/**
 * @brief Helper class to write IMU data to a ROS message.
 */
class IMUMsgWriter {
private:
  sensor_msgs__msg__Imu *msg;

public:
  /**
   * @brief Constructor for IMUMsgWriter.
   *
   * @param msg Pointer to the sensor_msgs__msg__Imu message structure to write
   * to.
   */
  IMUMsgWriter(sensor_msgs__msg__Imu *msg);

  /**
   * @brief Populates the ROS message with IMU data.
   *
   * @param imuData Pointer to the IMUData object.
   */
  void write(IMUData *imuData) const;
};