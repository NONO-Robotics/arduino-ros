#include "WToPWMConverter.h"

WToPWMConverter::WToPWMConverter(float maxW, int minPWM, int maxPWM)
{
    this->maxW = maxW;
    this->minPWM = minPWM;
    this->maxPWM = maxPWM;
}

int WToPWMConverter::convert(float w)
{
    if (std::abs(w) < 0.01)
        return 0;

    float ratio = std::abs(w) / this->maxW;

    // Mapea ese porcentaje al rango de PWM [PwmMin, PwmMax]
    int targetPWM = constrain(
        minPWM + ratio * (maxPWM - minPWM),
        minPWM,
        maxPWM);

    // Devuelve el PWM con el signo correcto
    return (w >= 0) ? targetPWM : -targetPWM;
}