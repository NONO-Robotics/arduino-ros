#include "IMUMsgWriter.h"

IMUMsgWriter::IMUMsgWriter(sensor_msgs__msg__Imu *msg)
{
    this->msg = msg;
}

void IMUMsgWriter::write(IMUData *imuData) const
{
    // Map the private data members of this class to the ROS message fields
    msg->orientation.w = imuData->getOrientationW();
    msg->orientation.x = imuData->getOrientationX();
    msg->orientation.y = imuData->getOrientationY();
    msg->orientation.z = imuData->getOrientationZ();

    msg->angular_velocity.x = imuData->getAngularVelocityX();
    msg->angular_velocity.y = imuData->getAngularVelocityY();
    msg->angular_velocity.z = imuData->getAngularVelocityZ();

    msg->linear_acceleration.x = imuData->getLinearAccelerationX();
    msg->linear_acceleration.y = imuData->getLinearAccelerationY();
    msg->linear_acceleration.z = imuData->getLinearAccelerationZ();
}