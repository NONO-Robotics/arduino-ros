#include "NavSatFixMsgWriter.h"

NavSatFixMsgWriter::NavSatFixMsgWriter(sensor_msgs__msg__NavSatFix *msg)
{
    this->msg = msg;
}

void NavSatFixMsgWriter::write(GPSData *gpsData)
{
    // Indicar siempre que el servicio es GPS
    msg->status.service = sensor_msgs__msg__NavSatStatus__SERVICE_GPS;

    // Usar el número de satélites para determinar el estado
    int num_satellites = gpsData->getSatellites();

    if (num_satellites < 3) // Menos de 3 satélites, no hay fix
    {
        msg->status.status = sensor_msgs__msg__NavSatStatus__STATUS_NO_FIX;
        msg->latitude = 0.0;
        msg->longitude = 0.0;
        msg->altitude = 0.0;
        msg->position_covariance_type = sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_UNKNOWN;
    }
    else // Tenemos suficientes satélites para una solución
    {
        // NOTA: TinyGPSPlus no distingue fácilmente entre fix 2D y 3D,
        // así que lo consideramos un fix general si hay 3 o más satélites.
        // Un sistema más avanzado podría intentar leer la sentencia GGA directamente.
        msg->status.status = sensor_msgs__msg__NavSatStatus__STATUS_FIX;

        // Rellenar los datos de posición
        msg->latitude = gpsData->getLatitude();
        msg->longitude = gpsData->getLongitude();
        msg->altitude = gpsData->getAltitude();

        // Calcular la covarianza como antes
        double hdop = gpsData->getHDOP();
        double position_variance = hdop * hdop;
        msg->position_covariance[0] = position_variance;
        msg->position_covariance[4] = position_variance;
        msg->position_covariance[8] = position_variance * 4.0;
        msg->position_covariance_type = sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_DIAGONAL_KNOWN;
    }
}
