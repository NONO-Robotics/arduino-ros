#include <RosNodeManager.h>

RosNodeManager::RosNodeManager(String nodeName,
#ifdef USE_WIFI_TRANSPORT
                               bool wifiEnergySavingMode,
                               wifi_power_t wifi_power,
#endif
                               bool syncTime,
                               int checkAgentConnectionIntervalMs,
                               int agentRequestTimeoutMs)
{
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

RosNodeManager *RosNodeManager::setup()
{
#ifdef USE_WIFI_TRANSPORT
  // ... (tu código WiFi actual se mantiene igual)
#elif defined(USE_SERIAL_TRANSPORT)
  delay(2000); // Dar tiempo al hardware para estabilizar la alimentación

  // 1. Limpiamos cualquier dato residual en el puerto serie antes de asignar el transporte
  while (Serial.available())
  {
    Serial.read();
  }

  set_microros_serial_transports(Serial);

  logger.info("Wait for micro-ROS agent (Serial)...");

  // 2. Bucle de ping más robusto
  bool agent_connected = false;
  while (!agent_connected)
  {
    // Intentamos hacer ping al agente (2 intentos de 1000ms cada uno)
    if (rmw_uros_ping_agent(1000, 2) == RMW_RET_OK)
    {
      agent_connected = true;
    }
    else
    {
      // Si falla, vaciamos el buffer de nuevo por si Ubuntu envió basura de inicialización USB
      while (Serial.available())
      {
        Serial.read();
      }
      delay(500);
    }
  }
  delay(1000); // Pequeño margen para que el agente estabilice su estado
#endif

  // 3. Manejo de errores sin bloqueos definitivos
  // En lugar de usar assertOk (que asumo detiene el código), evaluamos el retorno.
  rcl_ret_t support_ret = rclc_support_init(&support, 0, NULL, &allocator);
  if (support_ret != RCL_RET_OK)
  {
    logger.error("Error al inicializar rclc_support. Reiniciando ESP32...");
    delay(1000);
    ESP.restart(); // Reiniciamos el ESP para limpiar estados de memoria y volver a empezar
  }

  char *charNodeName = toCharArray(nodeName);

  rcl_ret_t node_ret = rclc_node_init_default(&node, charNodeName, "", &support);
  if (node_ret != RCL_RET_OK)
  {
    logger.error("Error al inicializar el nodo. Reiniciando ESP32...");
    delay(1000);
    ESP.restart();
  }

  delete charNodeName;

  rcl_ret_t exec_ret = rclc_executor_init(&executor, &support.context, 1, &allocator);
  if (exec_ret != RCL_RET_OK)
  {
    logger.error("Error al crear executor. Reiniciando ESP32...");
    delay(1000);
    ESP.restart();
  }

  logger.info("Connected to Ros2 Agent and Entities Created Successfully");
  return this;
}

rclc_support_t *RosNodeManager::getSupport() { return &support; };

rcl_node_t *RosNodeManager::getNode() { return &node; };

rcl_allocator_t *RosNodeManager::getAllocator() { return &allocator; }

rclc_executor_t *RosNodeManager::getExecutor() { return &executor; }

bool RosNodeManager::update(const uint64_t timeout_ns)
{
  checkAgentConnection->update();
#ifdef USE_WIFI_TRANSPORT
  wifiResetDetector.update();
#endif

  if (checkAgentConnection->hasBeenReached())
  {
    if (!this->isConnected(agentRequestTimeoutMs))
      this->restart();

    checkAgentConnection->reset();
  }

  return assertOk(rclc_executor_spin_some(&executor, timeout_ns),
                  "Cant't Node Manager state");
}

bool RosNodeManager::isConnected(const int timeout_ms, const uint8_t attempts)
{
  return rmw_uros_ping_agent(timeout_ms, attempts) == RMW_RET_OK;
}

void RosNodeManager::restart()
{
  logger.error("Connection to Micro-ROS Agent Lost!");
  logger.info("Restart ROS Node...");
  ESP.restart();
}