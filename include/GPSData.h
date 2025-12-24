#pragma once

#include <sensor_msgs/msg/nav_sat_fix.h>
#include <geometry_msgs/msg/vector3.h>
#include <TinyGPSPlus.h>

class GPSData
{
private:
    TinyGPSPlus data;

public:
    void encode(char c);
    bool isLocationUpdated() const;

    bool isLocationValid() const;

    double getLatitude();

    double getLongitude();

    double getAltitude();

    int getSatellites();

    // Devuelve el HDOP (Horizontal Dilution of Precision) para covarianza
    double getHDOP();

    int getTimeMinute();

    int getTimeSecond();

    int getTimeHour();

    void writeTo(sensor_msgs__msg__NavSatFix &msg);
};
