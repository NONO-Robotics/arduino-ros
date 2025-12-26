#pragma once

#include <GPSData.h>
#include <sensor_msgs/msg/nav_sat_fix.h>

class NavSatFixMsgWriter
{
private:
    sensor_msgs__msg__NavSatFix *msg;

public:
    NavSatFixMsgWriter(sensor_msgs__msg__NavSatFix *msg);
    void write(GPSData *gpsData);
};
