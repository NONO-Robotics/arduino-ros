#include "VelocityConverter.h"

VelocityConverter::VelocityConverter(
    float maxWInRadSeg,
    int pwmResolutionInBits,
    int minPwm)
{
  this->maxWInRadSeg = maxWInRadSeg;
  // Calculamos el valor máximo basado en bits (ej. 12 bits -> 4095)
  this->maxPwm = (int)((1UL << pwmResolutionInBits) - 1);
  this->minPwm = minPwm;
};

int VelocityConverter::wToSignedPWM(float w)
{
  // Zona muerta de software absoluta (ruido cero)
  if (abs(w) < 0.01)
    return 0;

  // Limitamos la entrada para no exceder la física del robot
  if (abs(w) > maxWInRadSeg)
    w = getSign(w) * maxWInRadSeg;

  // Cálculo de mapeo con aritmética de punto flotante para precisión
  // PWM = ( |Omega Actual| / Omega Máxima ) * MaxPWM_Counts
  float pwm = (abs(w) / maxWInRadSeg) * (float)maxPwm;

  // Mapeo de Zona Muerta del Motor (Deadzone compensation)
  // Si el cálculo da 1 pero el motor necesita 50 para moverse, ajustamos.
  if (pwm > 0 && pwm < minPwm)
  {
    pwm = minPwm;
  }

  // Clamp final de seguridad
  if (pwm > maxPwm)
    pwm = maxPwm;

  return getSign(w) * (int)pwm;
};

int VelocityConverter::getSign(float value)
{
  return value > 0 ? 1 : -1;
}