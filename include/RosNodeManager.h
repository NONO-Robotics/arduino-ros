#pragma once
#include "WifiConnectionManager.h"
#include <DeltaTimeComputer.h>
#include <Logger.h>
#include <RosUtils.h>
#include <StringUtils.h>
#include <WifiResetDetector.h>
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
  bool wifiEnergySavingMode;
  wifi_power_t wifi_power;
  bool syncTime;
  WifiConnectionManager *wifiConnectionManager;
  WifiResetDetector wifiResetDetector;
  DeltaTimeComputer *checkAgentConnection;
  int agentRequestTimeoutMs;

public:
  /**
   * @brief Constructor for RosNodeManager.
   *
   * @param nodeName Name of the ROS node.
   */
  RosNodeManager(String nodeName, bool wifiEnergySavingMode = false,
                 wifi_power_t wifi_power = WIFI_POWER_20_5dBm,
                 bool syncTime = true,
                 const int checkAgentConnectionIntervalMs = 10000,
                 const int agentRequestTimeoutMs = 5000);

  bool isConnected(const int timeout_ms = 500, const uint8_t attempts = 2);

  RosNodeManager *setup();

  bool update(const uint64_t timeout_ns = RCL_MS_TO_NS(2));

  rclc_support_t *getSupport();

  rcl_node_t *getNode();

  rcl_allocator_t *getAllocator();

  rclc_executor_t *getExecutor();

  void reset();
};