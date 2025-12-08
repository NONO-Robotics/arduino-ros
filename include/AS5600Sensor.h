#pragma once
#include <Arduino.h>
#include <Wire.h>

#define AS5600_DEFAULT_ADDR 0x36
#define AS5600_ERROR_VALUE  0xFFFF

class AS5600Sensor
{
private:
    int _value;
    int _address;
    TwoWire* _i2cPort;
public:
    AS5600Sensor(TwoWire* i2cPort = &Wire, int address = AS5600_DEFAULT_ADDR);

    bool begin();
    bool isSuccessful() const;
    int getValue();
    int update(); 
};