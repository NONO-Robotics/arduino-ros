#pragma once
#include "BLDCMotorController.h"
#include "BLDCMotorBuilder.h"
#include "FWAngularSpeed.h"

class FourWheelBLDCController
{
private:
    BLDCMotorController *motorFrontRightController;
    BLDCMotorController *motorFrontLeftController;
    BLDCMotorController *motorBackLeftController;
    BLDCMotorController *motorBackRightController;

public:
    FourWheelBLDCController(
        float maxW,
        int minPwm,
        int maxPwm,
        int pinPwmFrontRight,
        int pinDirFrontRight,
        int pinBrakeFrontRight,
        int pinPwmFrontLeft,
        int pinDirFrontLeft,
        int pinBrakeFrontLeft,
        int pinPwmBackRight,
        int pinDirBackRight,
        int pinBrakeBackRight,
        int pinPwmBackLeft,
        int pinDirBackLeft,
        int pinBrakeBackLeft);

    void stop();

    void applySpeed(FWAngularSpeed *fwAngularSpeed);
};