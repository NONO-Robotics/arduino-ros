#include "BLDCMotor.h"

// Constructor
BLDCMotor::BLDCMotor(
    int pwmPin,
    int dirPin,
    int brakePin,
    int resolutionInBits,
    int channel,
    float frequency,
    bool invertDirection)
    : pwmPin(pwmPin),
      dirPin(dirPin),
      brakePin(brakePin),
      channel(channel),
      frequency(frequency),
      resolutionInBits(resolutionInBits),
      invertDirection(invertDirection)
{
    currentSpeed = 0;
}

int BLDCMotor::getResolutionInBits()
{
    return resolutionInBits;
}

// Pin Initialization
BLDCMotor *BLDCMotor::setup()
{
    pinMode(pwmPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(brakePin, OUTPUT);

    // Configuración correcta para ESP32 Core v2.x (PlatformIO default)
    ledcSetup(channel, frequency, resolutionInBits);
    ledcAttachPin(pwmPin, channel);

    pwmMax = (1 << resolutionInBits) - 1;

    releaseBrake();
    return this;
}

int getSign(float value)
{
    return (value >= 0) ? 1 : -1;
}

// Set Speed and Direction
BLDCMotor *BLDCMotor::setPwmSpeed(int speed)
{
    // Limitamos el valor al rango permitido (ej. -4095 a 4095)
    speed = constrain(speed, -pwmMax, pwmMax);

    // 1. Caso Velocidad Cero
    if (speed == 0)
    {
        ledcWrite(channel, 0);
        this->currentSpeed = 0;
        return this; // No activamos freno físico aquí, solo inercia
    }

    // 2. Control de Dirección
    // Nota: Quitamos la lógica de stop() automático para evitar bloqueos.
    // El controlador de Rampa superior se encargará de pasar por 0 suavemente.
    if (speed > 0)
    {
        releaseBrake();
        digitalWrite(dirPin, invertDirection ? DIR_REVERSE: DIR_FORWARD);
    }
    else 
    {
        digitalWrite(dirPin, invertDirection ? DIR_FORWARD: DIR_REVERSE);
        releaseBrake();
    }

    // 3. Escritura PWM (SOLO ledcWrite)
    // Usamos abs() porque el PWM duty cycle siempre es positivo
    ledcWrite(channel, abs(speed));

    this->currentSpeed = speed;
    return this;
}

BLDCMotor *BLDCMotor::brake()
{
    digitalWrite(brakePin, HIGH); // Activa freno físico
    ledcWrite(channel, 0);        // Asegura PWM en 0
    this->currentSpeed = 0;
    return this;
}

BLDCMotor *BLDCMotor::releaseBrake()
{
    digitalWrite(brakePin, LOW);  // Libera freno
    // No escribimos PWM aquí, esperamos a la siguiente llamada de setPwmSpeed
    return this;
}

BLDCMotor *BLDCMotor::stop()
{
    // Stop simplemente activa el freno sin esperar tiempo
    this->brake();
    return this;
};