#pragma once

#include "BLDCMotor.h"

class BLDCMotorBuilder {
public:
    BLDCMotorBuilder(int pwmPin, int dirPin, int brakePin)
        : pwmPin(pwmPin),
          dirPin(dirPin),
          brakePin(brakePin),
          resolutionInBits(DEFAULT_RESOLUTION_IN_BITS),
          channel(DEFAULT_CHANNEL),
          frequency(DEFAULT_FREQUENCY),
          _invertDirection(false) {}

    BLDCMotorBuilder* setResolutionInBits(int bits) {
        if (bits <= 0 || bits > 16) { // Ejemplo de validación
            // Podrías lanzar una excepción o registrar un error
        }
        this->resolutionInBits = bits;
        return this;
    }

    BLDCMotorBuilder* setChannel(int ch) {
        this->channel = ch;
        return this;
    }

    BLDCMotorBuilder* setFrequency(float freq) {
        this->frequency = freq;
        return this;
    }

    BLDCMotorBuilder* invertDirection() {
        this->_invertDirection = true;
        return this;
    }

    BLDCMotor* build() {
        if (pwmPin < 0 || dirPin < 0 || brakePin < 0) {
             throw std::runtime_error("Pins must be non-negative.");
        }

        return new BLDCMotor(
            pwmPin,
            dirPin,
            brakePin,
            resolutionInBits,
            channel,
            frequency,
            _invertDirection
        );
    }

private:
    int pwmPin;
    int dirPin;
    int brakePin;

    int resolutionInBits;
    int channel;
    float frequency;
    bool _invertDirection;
};