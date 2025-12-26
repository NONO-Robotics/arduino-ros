#pragma once

#include <GPSData.h>

class NavSatFixMsgWirter
{
private:
    sensor_msgs__msg__NavSatFix *msg;

public:
    NavSatFixMsgWirter(sensor_msgs__msg__NavSatFix *msg);
    void write(GPSData *gpsData);
}
