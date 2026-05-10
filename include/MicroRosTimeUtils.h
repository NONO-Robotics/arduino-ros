#pragma once
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/header.h>
#include <rosidl_runtime_c/string_functions.h>

/**
 * @brief Utility for handling time synchronization in micro-ROS.
 */
class MicroRosTimeUtils {
public:
    /**
     * @brief Synchronizes time with the agent (Mini PC). Should be called in setup().
     * @param timeout_ms Maximum wait time in milliseconds.
     */
    static bool syncSession(int timeout_ms = 500);

    static bool syncSessionWithRetry(int timeout_ms = 500, int attempts = 50);

    /**
     * @brief Checks if the ESP32 clock is synchronized with the agent.
     */
    static bool isSynchronized();

    /**
     * @brief Assigns the current time and frame_id to the provided ROS 2 message header.
     * @param header Pointer to the ROS 2 message header.
     */
    static void setCurrentStamp(std_msgs__msg__Header* header);

private:
    unsigned long _last_sync_try = 0; 
    const unsigned long SYNC_INTERVAL = 5000; // 5 seconds
};