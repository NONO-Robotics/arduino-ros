#pragma once
#include <MultiResetDetector.h>
#include <WiFiManager.h>

class WifiResetDetector
{
private:
    MultiResetDetector *mrd;

public:
    WifiResetDetector(uint32_t windowMs = 20000, uint8_t targetResets = 3)
    {
        mrd = new MultiResetDetector(windowMs, targetResets);
    }

    void setup()
    {
        if (mrd->detect())
        {
            logger.info("Resetting WiFi settings...");
            WiFiManager wm;
            wm.resetSettings();
        }
    }

    void update()
    {
        mrd->process();
    }
};