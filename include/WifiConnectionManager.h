#pragma once

#ifdef USE_WIFI_TRANSPORT
#include "Logger.h"
#include "RosUtils.h"
#include <ConfigStorage.h>
#include <WiFiManager.h>

#define AGENT_IP_KEY "microRossAgentIp"
#define AGENT_PORT_KEY "microRossAgentPort"

/**
 * @brief Class for managing Wi-Fi connections and micro-ROS agent
 * configuration.
 */
class WifiConnectionManager {
private:
  String hostname;
  bool energySavingMode;
  wifi_power_t wifi_power;
  WiFiManager wifiManager;
  WiFiManagerParameter *microRossAgentIp;
  WiFiManagerParameter *microRossAgentPort;
  ConfigStorage *storage;

public:
  /**
   * @brief Constructor for WifiConnectionManager.
   *
   * @param hostname Hostname for the device.
   * @param energySavingMode Whether to enable Wi-Fi energy saving mode.
   * @param wifi_power Wi-Fi transmit power level.
   * @param connectTimeoutInSecs (Optional) Timeout for Wi-Fi connection in
   * seconds. Default is 30.
   * @param configPortalTimeoutInSecs (Optional) Timeout for configuration
   * portal in seconds. Default is 600.
   * @param configPath (Optional) Path to the configuration file. Default is
   * "/wifi_manager_config.json".
   */
  WifiConnectionManager(String hostname, bool energySavingMode,
                        wifi_power_t wifi_power,
                        unsigned long connectTimeoutInSecs = 30,
                        unsigned long configPortalTimeoutInSecs = 600,
                        String configPath = "/wifi_manager_config.json");

  /** @brief Destroy this manager and release allocated configuration storage. */
  ~WifiConnectionManager();

  /**
   * @brief Connects to the Wi-Fi network and configures the micro-ROS agent.
   *
   * Starts the WiFiManager to connect to a saved network or creates an access
   * point for configuration. Also retrieves the micro-ROS agent IP and port.
   */
  void connect();

  /**
   * @brief Checks if agent information was previously saved.
   *
   * @return true if agent IP and port are found in storage, false otherwise.
   */
  bool foundPreviouslySavedAgentInfo();

  /**
   * @brief Sets up the micro-ROS transport using the configured agent IP and
   * port.
   */
  void rosTransportSetup();
};
#endif
