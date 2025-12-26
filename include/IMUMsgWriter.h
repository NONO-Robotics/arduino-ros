#pragma once
#include "IMUData.h"
#include <sensor_msgs/msg/imu.h> 

class IMUMsgWriter
{
private:
    sensor_msgs__msg__Imu *msg;

public:
    IMUMsgWriter(sensor_msgs__msg__Imu *msg);
    void write(IMUData *imuData) const;
};