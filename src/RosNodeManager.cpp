#include <RosNodeManager.h>

RosNodeManager::RosNodeManager(String nodeName, 
#ifdef USE_WIFI_TRANSPORT
                               bool wifiEnergySavingMode,
                               wifi_power_t wifi_power,
#endif
                               bool syncTime,
                               int checkAgentConnectionIntervalMs,
                               int agentRequestTimeoutMs) {
  this->nodeName = nodeName;
#ifdef USE_WIFI_TRANSPORT
  this->wifiEnergySavingMode = wifiEnergySavingMode;
  this->wifi_power = wifi_power;

  wifiConnectionManager =
      new WifiConnectionManager(nodeName, wifiEnergySavingMode, wifi_power);
#endif
  this->syncTime = syncTime;
  this->agentRequestTimeoutMs = agentRequestTimeoutMs;
  checkAgentConnection = new DeltaTimeComputer(checkAgentConnectionIntervalMs);
  checkAgentConnection->reset();
}

RosNodeManager *RosNodeManager::setup() {
#ifdef USE_WIFI_TRANSPORT
  wifiResetDetector.setup();
  logger.info("Wait for wifi connection...");
  this->wifiConnectionManager->connect();
  if (syncTime) {
    syncClockTimeStamp(AR_UTC_TIME_OFFSET_IN_SECONDS);
  }
#elif defined(USE_SERIAL_TRANSPORT)
  Serial.begin(115200);
  set_microros_serial_transports(Serial);

  logger.info("Wait for micro-ROS agent (Serial)...");
  while (!this->isConnected(500, 1)) {
    delay(500);
  }
#endif

  assertOk(rclc_support_init(&support, 0, NULL, &allocator),
           "Can't create support for node: " + nodeName);

  char *charNodeName = toCharArray(nodeName);

  assertOk(rclc_node_init_default(&node, charNodeName, "", &support),
           "Can't create micro-ros node: " + nodeName);

  delete charNodeName;

  assertOk(rclc_executor_init(&executor, &support.context, 1, &allocator),
           "Error to create executor for node: " + nodeName);

  logger.info("Connected to Ros2 Agent");
  return this;
}

rclc_support_t *RosNodeManager::getSupport() { return &support; };

rcl_node_t *RosNodeManager::getNode() { return &node; };

rcl_allocator_t *RosNodeManager::getAllocator() { return &allocator; }

rclc_executor_t *RosNodeManager::getExecutor() { return &executor; }

bool RosNodeManager::update(const uint64_t timeout_ns) {
  checkAgentConnection->update();
#ifdef USE_WIFI_TRANSPORT
  wifiResetDetector.update();
#endif

  if (checkAgentConnection->hasBeenReached()) {
    if (!this->isConnected(agentRequestTimeoutMs))
      this->restart();

    checkAgentConnection->reset();
  }

  return assertOk(rclc_executor_spin_some(&executor, timeout_ns),
                  "Cant't Node Manager state");
}

bool RosNodeManager::isConnected(const int timeout_ms, const uint8_t attempts) {
  return rmw_uros_ping_agent(timeout_ms, attempts) == RMW_RET_OK;
}

void RosNodeManager::restart() {
  logger.error("Connection to Micro-ROS Agent Lost!");
  logger.info("Restart ROS Node...");
  ESP.restart();
}