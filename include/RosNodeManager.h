#pragma once
#include <micro_ros_platformio.h>
#ifdef USE_WIFI_TRANSPORT
#include "WifiConnectionManager.h"
#include <WifiResetDetector.h>
#endif
#include <DeltaTimeComputer.h>
#include <Logger.h>
#include <RosUtils.h>
#include <StringUtils.h>
#include <rclc/executor.h>
#include <timestamp.h>

/**
 * @brief Class for managing a ROS node.
 *
 * This class provides methods to initialize, set up, and update a ROS node
 * using the micro-ROS framework. It also manages the executor and allocator
 * for the node.
 */
class RosNodeManager {
private:
  String nodeName;
  rclc_support_t support;
  rcl_allocator_t allocator = rcl_get_default_allocator();
  rcl_node_t node;
  rclc_executor_t executor;
#ifdef USE_WIFI_TRANSPORT
  bool wifiEnergySavingMode;
  wifi_power_t wifi_power;
  WifiConnectionManager *wifiConnectionManager;
  WifiResetDetector wifiResetDetector;
#endif
  bool syncTime;
  DeltaTimeComputer *checkAgentConnection;
  int agentRequestTimeoutMs;

  void restart();

public:
  /**
   * @brief Constructor for RosNodeManager.
   *
   * @param nodeName Name of the ROS node.
   * @param wifiEnergySavingMode (Optional) Enable Wi-Fi energy saving mode. Default is false.
   * @param wifi_power (Optional) Wi-Fi transmit power. Default is WIFI_POWER_20_5dBm.
   * @param syncTime (Optional) Whether to synchronize time with the agent. Default is true.
   * @param checkAgentConnectionIntervalMs (Optional) Interval in milliseconds to check agent connection. Default is 10000ms.
   * @param agentRequestTimeoutMs (Optional) Timeout in milliseconds for agent requests. Default is 5000ms.
   */
  RosNodeManager(String nodeName, 
#ifdef USE_WIFI_TRANSPORT
                 bool wifiEnergySavingMode = false,
                 wifi_power_t wifi_power = WIFI_POWER_20_5dBm,
#endif
                 bool syncTime = true,
                 const int checkAgentConnectionIntervalMs = 10000,
                 const int agentRequestTimeoutMs = 5000);

  /**
   * @brief Checks if the node is connected to the micro-ROS agent.
   * 
   * @param timeout_ms (Optional) Timeout for the ping in milliseconds. Default is 500ms.
   * @param attempts (Optional) Number of ping attempts. Default is 2.
   * @return true if connected, false otherwise.
   */
  bool isConnected(const int timeout_ms = 500, const uint8_t attempts = 2);

  /**
   * @brief Sets up the Wi-Fi connection and initializes the micro-ROS node.
   * 
   * This method attempts to connect to the Wi-Fi network and then initializes
   * the micro-ROS node, allocator, and executor.
   * 
   * @return Pointer to the RosNodeManager instance.
   */
  RosNodeManager *setup();

  /**
   * @brief Updates the node executor to process callbacks.
   * 
   * This method should be called in the main loop to keep the ROS node active
   * and process incoming messages.
   * 
   * @param timeout_ns (Optional) Execution timeout in nanoseconds. Default is 2ms.
   * @return true if the update was successful, false if the agent is disconnected.
   */
  bool update(const uint64_t timeout_ns = RCL_MS_TO_NS(2));

  /**
   * @brief Get the micro-ROS support structure.
   * 
   * @return Pointer to the rclc_support_t structure.
   */
  rclc_support_t *getSupport();

  /**
   * @brief Get the ROS node structure.
   * 
   * @return Pointer to the rcl_node_t structure.
   */
  rcl_node_t *getNode();

  /**
   * @brief Get the micro-ROS allocator.
   * 
   * @return Pointer to the rcl_allocator_t structure.
   */
  rcl_allocator_t *getAllocator();

  /**
   * @brief Get the micro-ROS executor.
   * 
   * @return Pointer to the rclc_executor_t structure.
   */
  rclc_executor_t *getExecutor();
};