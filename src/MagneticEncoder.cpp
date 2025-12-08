#include "MagneticEncoder.h"

MagneticEncoder::MagneticEncoder(
    OnUpdateWEvent cb,
    short int channel,
    unsigned long sampleIntervalMs,
    double alpha,
    bool applyFilter,
    float deadZone,
    int address,
    TwoWire *i2cPort) : sensor(sensor), // Inicializa el puntero al sensor
                        sampleIntervalMs(sampleIntervalMs),
                        onUpdateEvent(cb), // Asigna la nueva función de callback
                        previousStep(0),   // Inicializa el valor anterior a 0
                        previousUpdateTimeMs(0)
{
  this->channel = channel;
  this->applyFilter = applyFilter;
  sensor = new AS5600Sensor(i2cPort, address);
  wCalculator = new WCalculator(alpha, deadZone);
  currentW = 0;
  currentStep = 0;
}

// Inicializa el sensor de la rueda
bool MagneticEncoder::begin()
{
  if (sensor->begin())
  {                                    // Intenta inicializar el sensor AS5600
    previousStep = sensor->getValue(); // Obtiene el valor inicial
    previousUpdateTimeMs = millis();   // Registra el tiempo de inicio
    return true;
  }
  else
  {
    logger.error("Encoder '" + String(channel) + "': Initialization fail.");
    return false;
  }
}

// Actualiza el estado del sensor de la rueda y calcula la velocidad angular
void MagneticEncoder::update()
{
  unsigned long currentTimeMs = millis(); // Obtiene el tiempo actual una vez

  // Verifica si ha pasado el intervalo de muestreo deseado
  if ((currentTimeMs - previousUpdateTimeMs) >= sampleIntervalMs)
  {
    sensor->update(); // Realiza una nueva lectura del sensor AS5600

    if (sensor->isSuccessful())
    {
      currentStep = sensor->getValue();

      // Calcula la diferencia de ángulo crudo, manejando el "wrap-around" (0-4095)
      int diffRaw = (int)currentStep - (int)previousStep;
      if (diffRaw > 2048)
      {                  // Si el salto es grande y positivo (ej. 4000 -> 100)
        diffRaw -= 4096; // Ajusta para el wrap-around
      }
      else if (diffRaw < -2048)
      {                  // Si el salto es grande y negativo (ej. 100 -> 4000)
        diffRaw += 4096; // Ajusta para el wrap-around
      }

      // Calcula la diferencia de tiempo en milisegundos
      unsigned long deltaTimeMs = currentTimeMs - previousUpdateTimeMs;

      currentW = wCalculator->getWInRadBySec(diffRaw, deltaTimeMs, this->applyFilter);

      onUpdateEvent(channel, currentStep, currentW);

      // Actualiza los valores anteriores para la próxima iteración,
      // independientemente de si se superó el umbral. Esto asegura que
      // la velocidad angular siempre se calcule desde la última lectura exitosa.
      previousStep = currentStep;
      previousUpdateTimeMs = currentTimeMs;
    }
    else
    {
      logger.error("Encoder '" + String(channel) + "': Cant read a value.");
    }
  }
}

short int MagneticEncoder::getChannel() { return channel; }

float MagneticEncoder::getW() { return currentW; }

uint16_t MagneticEncoder::getStep() { return currentStep; }
