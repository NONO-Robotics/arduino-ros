#pragma once
#include "AS5600Sensor.h"
#include <Arduino.h>
#include <math.h>
#include <AS5600Sensor.h>
#include <WCalculator.h>
#include <Logger.h>

typedef void (*OnUpdateWEvent)(short int channel, int step, float w);

const int DEFAULT_SAMPLE_INTERVAL_MS = 50;

class MagneticEncoder
{

private:
  bool applyFilter;
  short int channel;
  AS5600Sensor *sensor;
  uint16_t previousStep;              // Almacena el valor del ángulo de la lectura anterior
  unsigned long previousUpdateTimeMs; // Almacena el tiempo en ms de la lectura anterior
  unsigned long sampleIntervalMs;     // Intervalo de muestreo deseado en milisegundos

  OnUpdateWEvent onUpdateEvent; // Función a llamar cuando la velocidad angular cambia

  WCalculator *wCalculator;
  float currentW;
  uint16_t currentStep;

public:
  MagneticEncoder(
      OnUpdateWEvent cb,
      short int channel = 0,
      unsigned long sampleIntervalMs = DEFAULT_SAMPLE_INTERVAL_MS,
      double alpha = DEFAULT_ALPHA,
      bool applyFilter = true,
      float deadZone = DEFAULT_DEAD_ZONE,
      int address = AS5600_DEFAULT_ADDR,
      TwoWire* i2cPort = &Wire
    );

  bool begin();
  void update();
  short int getChannel();
  float getW();
  uint16_t getStep();
};