#include "IMUData.h"

IMUData::IMUData() : 
    orientation_x(0.0), orientation_y(0.0), orientation_z(0.0), orientation_w(1.0), // Default to a valid quaternion
    angular_velocity_x(0.0), angular_velocity_y(0.0), angular_velocity_z(0.0),
    linear_acceleration_x(0.0), linear_acceleration_y(0.0), linear_acceleration_z(0.0)
{}

void IMUData::setOrientation(double w, double x, double y, double z) {
    orientation_w = w;
    orientation_x = x;
    orientation_y = y;
    orientation_z = z;
}

void IMUData::setAngularVelocity(double x, double y, double z) {
    angular_velocity_x = x;
    angular_velocity_y = y;
    angular_velocity_z = z;
}

void IMUData::setLinearAcceleration(double x, double y, double z) {
    linear_acceleration_x = x;
    linear_acceleration_y = y;
    linear_acceleration_z = z;
}

void IMUData::writeTo(sensor_msgs__msg__Imu& msg) const {
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