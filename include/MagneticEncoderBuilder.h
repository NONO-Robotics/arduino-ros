#pragma once

#include "MagneticEncoder.h"
#include "WCalculator.h"

class MagneticEncoderBuilder
{
private:
    OnUpdateWEvent _callback;
    short int _channel;
    unsigned long _sampleIntervalMs;
    double _alpha;
    bool _applyFilter;
    float _deadZone;
    int _address;
    TwoWire* _i2cPort;

public:
    MagneticEncoderBuilder();

    MagneticEncoderBuilder& setCallback(OnUpdateWEvent cb);
    MagneticEncoderBuilder& setChannel(short int channel);
    MagneticEncoderBuilder& setSampleInterval(unsigned long ms);
    MagneticEncoderBuilder& setAlpha(double alpha);
    MagneticEncoderBuilder& withFilter(bool enable);
    MagneticEncoderBuilder& setDeadZone(float deadZone);
    MagneticEncoderBuilder& setI2CAddress(int address);
    MagneticEncoderBuilder& setI2CPort(TwoWire* i2cPort);

    MagneticEncoder* build();
};