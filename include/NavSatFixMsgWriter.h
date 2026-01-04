#pragma once

#include <GPSData.h>
#include <sensor_msgs/msg/nav_sat_fix.h>

/**
 * @brief Helper class to write GPS data to a ROS NavSatFix message.
 */
class NavSatFixMsgWriter {
private:
  sensor_msgs__msg__NavSatFix *msg;

public:
  /**
   * @brief Constructor for NavSatFixMsgWriter.
   *
   * @param msg Pointer to the sensor_msgs__msg__NavSatFix message structure to
   * write to.
   */
  NavSatFixMsgWriter(sensor_msgs__msg__NavSatFix *msg);

  /**
   * @brief Populates the ROS message with GPS data.
   *
   * @param gpsData Pointer to the GPSData object.
   */
  void write(GPSData *gpsData);
};
