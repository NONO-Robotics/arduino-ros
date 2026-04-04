#pragma once
#include "MicroRosPublisher.h"
#include "RosMessage.h"

/**
 * @brief Clase para publicar vectores 3D con estampa de tiempo usando micro-ROS.
 *
 * Ideal para publicar velocidades de ruedas donde x = rueda izquierda, y = rueda derecha,
 * garantizando la sincronización temporal con Nav2 y TF.
 */
class Vector3StampedPublisher {
public:
    /**
     * @brief Constructor para Vector3StampedPublisher.
     * @param publisher Puntero al objeto MicroRosPublisher subyacente.
     * @param frameId Nombre del marco de referencia (ej. "base_link" o "odom").
     */
    Vector3StampedPublisher(
        MicroRosPublisher *publisher, 
        String frameId = "base_link");

    ~Vector3StampedPublisher();

    /**
     * @brief Publica el vector con la estampa de tiempo actual.
     * @param x Valor para el eje X (ej. velocidad rueda izquierda).
     * @param y Valor para el eje Y (ej. velocidad rueda derecha).
     * @param z Valor para el eje Z (por defecto 0.0 para un robot diferencial).
     */
    void publish(float x = 0.0, float y = 0.0, float z = 0.0);

private:
    MicroRosPublisher *publisher;
    geometry_msgs__msg__Vector3Stamped *msg;
};