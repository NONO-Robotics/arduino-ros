#pragma once
#include "RosNodeManager.h"
#include <DeltaTimeComputer.h>

/**
 * @brief Handles restarting the ROS node manager if connection is lost.
 */
class RosNodeManagerRestartHandler {
private:
  RosNodeManager *nodeManager;
  const int timeout_ms;
  DeltaTimeComputer *checkConnection;

  void restart();

public:
  /**
   * @brief Constructor.
   * @param nodeManager Pointer to RosNodeManager.
   * @param checkConnectionIntervalMs Interval to check connection.
   * @param timeout_ms Timeout for connection check.
   */
  RosNodeManagerRestartHandler(RosNodeManager *nodeManager,
                               const int checkConnectionIntervalMs = 10000,
                               const int timeout_ms = 5000);

  /**
   * @brief Update loop to check connection and restart if needed.
   */
  void update();
};