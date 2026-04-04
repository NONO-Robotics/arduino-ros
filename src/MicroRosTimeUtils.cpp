#include "MicroRosTimeUtils.h"
#include "Logger.h"

bool MicroRosTimeUtils::syncSession(int timeout_ms) {
    rmw_uros_sync_session(timeout_ms);

    if (MicroRosTimeUtils::isSynchronized()) {
        logger.info("Time syncronized");
        return true;
    } else {
        logger.error("Error to syncronice time.");
        return false;
    }
}

bool MicroRosTimeUtils::isSynchronized() {
    return rmw_uros_epoch_synchronized();
}

void MicroRosTimeUtils::setCurrentStamp(std_msgs__msg__Header* header) {
    // Si perdimos la sincronización, intentamos recuperarla RÁPIDO
    if (!isSynchronized()) {
        // Intentamos resincronizar con un timeout corto (ej. 10 ms)
        // para no bloquear el control de motores del ESP32
        rmw_uros_sync_session(10); 
    }

    // Volvemos a comprobar por si la línea anterior tuvo éxito
    if (isSynchronized()) {
        int64_t time_ms = rmw_uros_epoch_millis();
        header->stamp.sec = (int32_t)(time_ms / 1000);
        header->stamp.nanosec = (uint32_t)((time_ms % 1000) * 1000000);
    } else {
        // Si seguimos sin sincronización, usar el tiempo local de hardware
        // (millis o micros) es marginalmente mejor que enviar 0 absoluto, 
        // aunque ROS 2 igual podría quejarse hasta que se recupere la conexión.
        int64_t local_time_ms = esp_timer_get_time() / 1000; 
        header->stamp.sec = (int32_t)(local_time_ms / 1000);
        header->stamp.nanosec = (uint32_t)((local_time_ms % 1000) * 1000000);
    }
}