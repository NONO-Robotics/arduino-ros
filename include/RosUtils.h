#pragma once
#include <StringUtils.h>
#include <micro_ros_platformio.h>
#include <rcl/allocator.h>
#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <rcl/time.h>
#include <rclc/rclc.h>
#include <rosidl_runtime_c/string_functions.h>

#ifdef USE_WIFI_TRANSPORT
#include <WiFi.h>
const wifi_power_t WIFI_POWER_20_5dBm = (wifi_power_t)82; // 20.5 dBm * 4 = 82
#endif


bool assertOk(rcl_ret_t result, String msg);

#ifdef USE_WIFI_TRANSPORT

/**
 * @brief Connect to the micro-ROS agent via Wi-Fi.
 *
 * This function connects to the micro-ROS agent using the provided Wi-Fi SSID,
 * password, IP address, and port number.
 *
 * @param hostname Hostname for the device.
 * @param wifi_ssid Wi-Fi SSID for connection.
 * @param wifi_pass Wi-Fi password for connection.
 * @param agent_ip IP address of the micro-ROS agent.
 * @param agent_port Port number of the micro-ROS agent (default is 8888).
 * @param energySavingMode Whether to enable Wi-Fi energy saving mode.
 * @param wifi_power Wi-Fi transmit power level.
 *
 */
void connect_to_agent_via_wifi(String hostname, String wifi_ssid,
                               String wifi_pass, String agent_ip,
                               uint16_t agent_port, bool energySavingMode,
                               wifi_power_t wifi_power);
#endif
