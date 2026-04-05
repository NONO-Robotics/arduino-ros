#pragma once
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/header.h>
#include <rosidl_runtime_c/string_functions.h>

/**
 * @brief Utilidad para el manejo de sincronización de tiempo en micro-ROS.
 */
class MicroRosTimeUtils {
public:
    /**
     * @brief Sincroniza el tiempo con el agente (Mini PC). Debe llamarse en el setup().
     * @param timeout_ms Tiempo máximo de espera en milisegundos.
     */
    static bool syncSession(int timeout_ms = 500);

    static bool syncSessionWithRetry(int timeout_ms = 500, int attempts = 50);

    /**
     * @brief Verifica si el reloj del ESP32 está sincronizado con el agente.
     */
    static bool isSynchronized();

    /**
     * @brief Asigna el tiempo actual y el frame_id al header proporcionado.
     * @param header Puntero al header del mensaje ROS 2.
     */
    static void setCurrentStamp(std_msgs__msg__Header* header);


private:
    unsigned long _last_sync_try = 0; 
    const unsigned long SYNC_INTERVAL = 5000; // 5 segundos
};