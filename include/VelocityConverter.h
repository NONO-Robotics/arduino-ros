#pragma once
#include <Arduino.h>

class VelocityConverter {
  private:
    float maxWInRadSeg;
    int minPwm, maxPwm;

  public:
    /**
     * @param maxWInRadSeg Velocidad angular máxima física del robot
     * @param pwmResolutionInBits Resolución del PWM (Recomendado: 12 bits)
     * @param minPwm Zona muerta mínima para que el motor empiece a girar
     */
    VelocityConverter(
      float maxWInRadSeg, 
      int pwmResolutionInBits, 
      int minPwm
    );;

    int wToSignedPWM(float w);
  
  private:
    int getSign(float value);
};