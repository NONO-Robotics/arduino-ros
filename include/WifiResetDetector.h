#pragma once
#include <MultiResetDetector.h>
#include <WiFiManager.h>

/**
 * @brief Class detects multiple resets to trigger Wi-Fi setting reset.
 */
class WifiResetDetector {
private:
  MultiResetDetector *mrd;

public:
  /**
   * @brief Constructor for WifiResetDetector.
   *
   * @param windowMs (Optional) Time window in milliseconds for detection.
   * Default is 10000ms.
   * @param targetResets (Optional) Number of resets to trigger action. Default
   * is 3.
   */
  WifiResetDetector(uint32_t windowMs = 10000, uint8_t targetResets = 3) {
    mrd = new MultiResetDetector(windowMs, targetResets);
  }

  /**
   * @brief Setup detection and reset Wi-Fi settings if condition met.
   */
  void setup() {
    if (mrd->detect()) {
      logger.info("Resetting WiFi settings...");
      WiFiManager wm;
      wm.resetSettings();
    }
  }

  /**
   * @brief Process reset detection. Should be called/used if library requires
   * it.
   */
  void update() { mrd->process(); }

  /**
   * @brief Stop detection.
   */
  void reset() { mrd->stop(); }
};