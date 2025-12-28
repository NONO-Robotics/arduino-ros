#pragma once

#include "RosUtils.h"
#include "Logger.h"
#include <WiFiManager.h>
#include <ConfigStorage.h>


#define AGENT_IP_KEY "microRossAgentIp"
#define AGENT_PORT_KEY "microRossAgentPort"

class WifiConnectionManager
{
private:
    String hostname;
    bool energySavingMode;
    wifi_power_t wifi_power;
    WiFiManager wifiManager;
    WiFiManagerParameter *microRossAgentIp;
    WiFiManagerParameter *microRossAgentPort;
    ConfigStorage *storage;
public:
    WifiConnectionManager(
        String hostname,
        bool energySavingMode,
        wifi_power_t wifi_power,
    unsigned long connectTimeoutInSecs = 30,
    unsigned long configPortalTimeoutInSecs = 600,
    String configPath = "/wifi_manager_config.json");

    ~WifiConnectionManager();

    void connect();

    bool foundPreviouslySavedAgentInfo();

    void rosTransportSetup();
};