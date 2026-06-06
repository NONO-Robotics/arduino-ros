#include "MicroRosTimeUtils.h"
#include "Logger.h"

bool MicroRosTimeUtils::syncSession(int timeout_ms) {
    if (rmw_uros_sync_session(timeout_ms)== RMW_RET_OK) {
        logger.info("Time syncronized");
        return true;
    } else {
        logger.error("Error to syncronice time.");
        return false;
    }
}

bool MicroRosTimeUtils::syncSessionWithRetry(int timeout_ms, int attempts) {
    bool synced = false;
    int i;
    for (i = 0; i < attempts; i++) {
        if (rmw_uros_sync_session(timeout_ms) == RMW_RET_OK) {
            synced = true;
            logger.info("Time Sync successful. Attempts: " + String(i+1));
            break;
        }
        delay(100);
        logger.info(".");
    }
    if (synced) {
        logger.info("Time Sync falled after " + String(i+1) + " attempts. Proceeding without time sync.");
    }
    return synced;
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
        // IMPORTANT: If the monitor still reports a 139s delay, 
        // it means this section is still executing.
        // As long as there is no sync, it is better to send 0 so Nav2 knows the data is not valid.
        header->stamp.sec = 0;
        header->stamp.nanosec = 0;
    }
}

