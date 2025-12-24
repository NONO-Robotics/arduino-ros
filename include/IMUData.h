// IMUData.h
#pragma once

#include <sensor_msgs/msg/imu.h> // We need the definition here

class IMUData
{
public:
    // Constructor
    IMUData();

    // Setters to modify private data
    void setOrientation(double w, double x, double y, double z);
    void setAngularVelocity(double x, double y, double z);
    void setLinearAcceleration(double x, double y, double z);

    double getOrientationX() const { return orientation_x; }
    double getOrientationY() const { return orientation_y; }
    double getOrientationZ() const { return orientation_z; }
    double getOrientationW() const { return orientation_w; }


    // The converter/populator method.
    // It takes a reference to a ROS message and fills it with the class's data.
    // It's marked 'const' because it doesn't modify the IMUData object itself.
    void writeTo(sensor_msgs__msg__Imu& msg) const;

private:
    // Data members are now private
    double orientation_x, orientation_y, orientation_z, orientation_w;
    double angular_velocity_x, angular_velocity_y, angular_velocity_z;
    double linear_acceleration_x, linear_acceleration_y, linear_acceleration_z;
};