#include "NavSatFixMsgWriter.h"

NavSatFixMsgWriter::NavSatFixMsgWriter(sensor_msgs__msg__NavSatFix *msg)
{
    this->msg = msg;
}

void NavSatFixMsgWriter::write(GPSData *gpsData)
{
    // Always indicate that the service is GPS
    msg->status.service = sensor_msgs__msg__NavSatStatus__SERVICE_GPS;

    // Use the number of satellites to determine the status
    int num_satellites = gpsData->getSatellites();

    if (num_satellites < 3) // Fewer than 3 satellites, no fix
    {
        msg->status.status = sensor_msgs__msg__NavSatStatus__STATUS_NO_FIX;
        msg->latitude = 0.0;
        msg->longitude = 0.0;
        msg->altitude = 0.0;
        msg->position_covariance_type = sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_UNKNOWN;
    }
    else // We have enough satellites for a solution
    {
        // NOTE: TinyGPSPlus does not easily distinguish between 2D and 3D fix,
        // so we consider it a general fix if there are 3 or more satellites.
        // A more advanced system could try to read the GGA sentence directly.
        msg->status.status = sensor_msgs__msg__NavSatStatus__STATUS_FIX;

        // Fill position data
        msg->latitude = gpsData->getLatitude();
        msg->longitude = gpsData->getLongitude();
        msg->altitude = gpsData->getAltitude();

        // Calculate covariance as before
        double hdop = gpsData->getHDOP();
        double position_variance = hdop * hdop;
        msg->position_covariance[0] = position_variance;
        msg->position_covariance[4] = position_variance;
        msg->position_covariance[8] = position_variance * 4.0;
        msg->position_covariance_type = sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_DIAGONAL_KNOWN;
    }
}
