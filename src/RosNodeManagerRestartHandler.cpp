
#include "RosNodeManagerRestartHandler.h"

RosNodeManagerRestartHandler::RosNodeManagerRestartHandler(
    RosNodeManager *nodeManager,
    const int checkConnectionIntervalMs,
    const int timeout_ms) : nodeManager(nodeManager), timeout_ms(timeout_ms)
{
    checkConnection = new DeltaTimeComputer(checkConnectionIntervalMs);
    checkConnection->reset();
}

void RosNodeManagerRestartHandler::update()
{
    if (checkConnection->hasBeenReached())
    {
        if (!nodeManager->isConnected(timeout_ms))
        {
            logger.error("Connection to Micro-ROS Agent Lost!");
            nodeManager->reset();
        }
        checkConnection->reset();
    }
    checkConnection->update();

    // Process incoming ROS messages and call callbacks
    if (!nodeManager->update())
    {
        logger.error("Connection to Micro-ROS Agent Lost!");
        nodeManager->reset();
    }
}