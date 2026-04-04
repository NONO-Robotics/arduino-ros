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
    if (isSynchronized()) {
        int64_t time_ms = rmw_uros_epoch_millis();
        header->stamp.sec = (int32_t)(time_ms / 1000);
        header->stamp.nanosec = (uint32_t)((time_ms % 1000) * 1000000);
    } else {
        // Si por alguna razón se pierde la sincronización, enviamos 0
        header->stamp.sec = 0;
        header->stamp.nanosec = 0;
    }
}