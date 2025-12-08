#pragma once
#include <Arduino.h>

const int DEFAULT_CHANNEL = 0;
const float DEFAULT_FREQUENCY = 20000; // Subido a 20kHz para silenciar el motor
const int DEFAULT_RESOLUTION_IN_BITS = 11;

class BLDCMotor {
public:
    BLDCMotor(
        int pwmPin, 
        int dirPin, 
        int brakePin,
        int resolutionInBits = DEFAULT_RESOLUTION_IN_BITS,
        int channel = DEFAULT_CHANNEL,
        float frequency = DEFAULT_FREQUENCY,
        bool invertDirection = false);

    BLDCMotor* setup();
    BLDCMotor* setPwmSpeed(int pwm);
    int getResolutionInBits();

    BLDCMotor* brake();     
    BLDCMotor* releaseBrake();
    BLDCMotor* stop(); // Eliminado el float pauseInMs

private:
    int pwmPin;
    int dirPin;
    int brakePin;
    int currentSpeed;
    int channel;
    float frequency;
    int resolutionInBits;
    int pwmMax;
    bool invertDirection;

    const int DIR_FORWARD = LOW;
    const int DIR_REVERSE = HIGH;
};