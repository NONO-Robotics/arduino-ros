#pragma once

#include <sensor_msgs/msg/imu.h> 

class IMUMsgWriter
{
private:
    sensor_msgs__msg__Imu *msg;

public:
    IMUMsgWriter(sensor_msgs__msg__Imu *msg);
    void writer(sensor_msgs__msg__Imu &msg) const;
};