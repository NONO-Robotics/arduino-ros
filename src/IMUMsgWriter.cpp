#include "IMUMsgWriter.h"

IMUMsgWriter::IMUMsgWriter(sensor_msgs__msg__Imu *msg)
{
    this->msg = msg;
}

void IMUMsgWriter::writeTo(sensor_msgs__msg__Imu &msg) const
{
    // Map the private data members of this class to the ROS message fields
    msg.orientation.w = this->orientation_w;
    msg.orientation.x = this->orientation_x;
    msg.orientation.y = this->orientation_y;
    msg.orientation.z = this->orientation_z;

    msg.angular_velocity.x = this->angular_velocity_x;
    msg.angular_velocity.y = this->angular_velocity_y;
    msg.angular_velocity.z = this->angular_velocity_z;

    msg.linear_acceleration.x = this->linear_acceleration_x;
    msg.linear_acceleration.y = this->linear_acceleration_y;
    msg.linear_acceleration.z = this->linear_acceleration_z;
}