#include "GPSData.h"

void GPSData::encode(char c)
{
    data.encode(c);
}

bool GPSData::isLocationUpdated() const
{
    return data.location.isUpdated();
}

bool GPSData::isLocationValid() const
{
    return data.location.isValid();
}

double GPSData::getLatitude()
{
    return data.location.lat();
}

double GPSData::getLongitude()
{
    return data.location.lng();
}

double GPSData::getAltitude()
{
    return data.altitude.meters();
}

int GPSData::getSatellites()
{
    return data.satellites.value();
}

// Devuelve el HDOP (Horizontal Dilution of Precision) para covarianza
double GPSData::getHDOP()
{
    return data.hdop.isValid() ? data.hdop.value() : 99.0;
}

int GPSData::getTimeMinute()
{
    return data.time.isValid() ? data.time.minute() : 0;
}

int GPSData::getTimeSecond()
{
    return data.time.isValid() ? data.time.second() : 0;
}

int GPSData::getTimeHour()
{
    return data.time.isValid() ? data.time.hour() : 0;
}

void GPSData::writeTo(sensor_msgs__msg__NavSatFix &msg)
{
    // Indicar siempre que el servicio es GPS
    msg.status.service = sensor_msgs__msg__NavSatStatus__SERVICE_GPS;

    // Usar el número de satélites para determinar el estado
    int num_satellites = this->getSatellites();

    if (num_satellites < 3) // Menos de 3 satélites, no hay fix
    {
        msg.status.status = sensor_msgs__msg__NavSatStatus__STATUS_NO_FIX;
        msg.latitude = 0.0;
        msg.longitude = 0.0;
        msg.altitude = 0.0;
        msg.position_covariance_type = sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_UNKNOWN;
    }
    else // Tenemos suficientes satélites para una solución
    {
        // NOTA: TinyGPSPlus no distingue fácilmente entre fix 2D y 3D,
        // así que lo consideramos un fix general si hay 3 o más satélites.
        // Un sistema más avanzado podría intentar leer la sentencia GGA directamente.
        msg.status.status = sensor_msgs__msg__NavSatStatus__STATUS_FIX;

        // Rellenar los datos de posición
        msg.latitude = this->getLatitude();
        msg.longitude = this->getLongitude();
        msg.altitude = this->getAltitude();

        // Calcular la covarianza como antes
        double hdop = this->getHDOP();
        double position_variance = hdop * hdop;
        msg.position_covariance[0] = position_variance;
        msg.position_covariance[4] = position_variance;
        msg.position_covariance[8] = position_variance * 4.0;
        msg.position_covariance_type = sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_DIAGONAL_KNOWN;
    }
}
