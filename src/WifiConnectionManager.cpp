#include "WifiConnectionManager.h"

#ifdef USE_WIFI_TRANSPORT
WifiConnectionManager::WifiConnectionManager(
    String hostname,
    bool energySavingMode,
    wifi_power_t wifi_power,
    unsigned long connectTimeoutInSecs,
    unsigned long configPortalTimeoutInSecs,
    String configPath)
{
    this->hostname = hostname;
    this->energySavingMode = energySavingMode;
    this->wifi_power = wifi_power;

    storage = new ConfigStorage(configPath);
    storage->begin();

    if (!foundPreviouslySavedAgentInfo())
    {
        wifiManager.resetSettings();
    }

    String agentIp = storage->get(AGENT_IP_KEY, "192.168.2.100");
    String agentPort = storage->get(AGENT_PORT_KEY, "8888");

    microRossAgentIp = new WiFiManagerParameter(AGENT_IP_KEY, "Micro ROS Agent IP", agentIp.c_str(), 15);
    microRossAgentPort = new WiFiManagerParameter(AGENT_PORT_KEY, "Micro ROS Agent Port", agentPort.c_str(), 5);
    wifiManager.addParameter(microRossAgentIp);
    wifiManager.addParameter(microRossAgentPort);
    wifiManager.setConnectTimeout(connectTimeoutInSecs);           // Espera 20 segundos antes de fallar
    wifiManager.setConfigPortalTimeout(configPortalTimeoutInSecs); // Cierra el portal tras 10 minutos de inactividad
    wifiManager.setCaptivePortalEnable(true);                      // Fuerza el portal cautivo
}

WifiConnectionManager::~WifiConnectionManager()
{
    delete microRossAgentIp;
    delete microRossAgentPort;
    delete storage;
}

void WifiConnectionManager::connect()
{
    if (!foundPreviouslySavedAgentInfo())
    {
        WiFi.disconnect(true); // Borra el estado de conexión temporal
        WiFi.mode(WIFI_OFF);   // Apaga el WiFi un momento
        delay(100);
    }

    bool res = wifiManager.autoConnect(hostname.c_str());

    if (!res || WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Failed to connect or hit timeout");
        ESP.restart();
    }

    if (!foundPreviouslySavedAgentInfo())
    {
        storage->set(AGENT_IP_KEY, String(microRossAgentIp->getValue()));
        storage->set(AGENT_PORT_KEY, String(microRossAgentPort->getValue()));
        storage->save();
    }

    logger.info("Micro ROS Agent IP: " + String(microRossAgentIp->getValue()));
    logger.info("Micro ROS Agent Port: " + String(microRossAgentPort->getValue()));

    WiFi.setHostname(toCharArray(hostname));

    if (!energySavingMode)
    {
        // Set ESP32 to station mode
        WiFi.mode(WIFI_STA);
        // Disable Wi-Fi power saving mode
        WiFi.setSleep(energySavingMode);
    }

    rosTransportSetup();

    logger.info("Wait for wifi connection...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    if (WiFi.setTxPower(wifi_power))
        logger.info("Setup TX Power...");
    else
        logger.error("TX Power: Setup error...");

    logger.info("Wifi connection stablished...");
}

void WifiConnectionManager::rosTransportSetup()
{
    logger.info("Set Micro Ros wifi transports...");

    IPAddress ip;
    ip.fromString(microRossAgentIp->getValue());

    static struct micro_ros_agent_locator locator = {ip, atoi(microRossAgentPort->getValue())};

    rmw_uros_set_custom_transport(
        false,
        (void *)&locator,
        platformio_transport_open,
        platformio_transport_close,
        platformio_transport_write,
        platformio_transport_read);
}

bool WifiConnectionManager::foundPreviouslySavedAgentInfo()
{
    return storage->has(AGENT_IP_KEY) && storage->has(AGENT_PORT_KEY);
}
#endif
