#pragma once

#include <GPSData.h>
#include "sensor_msgs/msg/nav_sat_fix.h"

class NavSatFixMsgWirter
{
private:
    sensor_msgs__msg__NavSatFix *msg;

public:
    NavSatFixMsgWirter(sensor_msgs__msg__NavSatFix *msg);
    void write(GPSData *gpsData);
};
