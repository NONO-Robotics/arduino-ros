<div align="center">
  <img src="https://github.com/adrianmarino/4w-ros-robot/blob/main/images/robot-v4.png" alt="Arduino ROS Robot Logo"/>
  
  # Arduino-ROS Library

  [![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Registry-red.svg)](https://registry.platformio.org/libraries/adrianmarino/arduino-ros)
  [![Framework](https://img.shields.io/badge/Framework-Arduino-blue.svg)](https://www.arduino.cc/)
  [![ROS 2](https://img.shields.io/badge/ROS%202-Humble-22314E.svg)](https://docs.ros.org/en/humble/index.html)
  [![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
</div>

---

**Arduino-ROS** provides a robust framework for integrating **micro-ROS** on ESP32 microcontrollers using **PlatformIO**. It facilitates node lifecycle management, Wi-Fi connection stability, typed message publishing/subscription, and hardware abstraction for differential drive robots (BLDC motors, encoders, and kinematics).

## 📖 Table of Contents

- [Ecosystem](#-ecosystem)
- [Installation](#-installation)
- [API Reference](#-api-reference)
  - [Core & Lifecycle](#core--lifecycle)
  - [Hardware Abstraction](#hardware-abstraction)
  - [Kinematics & Odometry](#kinematics--odometry)
  - [Communication](#communication)
  - [Utilities](#utilities)
- [Usage Example](#-usage-example)
- [Build](#-build)

---

## 🌐 Ecosystem

This library is part of the **4w-ros-robot** project family:

| Component | Project | Description |
|-----------|---------|-------------|
| **Core** | [4w-ros-robot](https://github.com/adrianmarino/4w-ros-robot) | Main project repository. |
| **Nav** | [4w-robot-ros-movement](https://github.com/adrianmarino/4w-robot-ros-movement.git) | Movement controller firmware. |
| **Sensors** | [4w-robot-ros-w-publisher](https://github.com/adrianmarino/4w-robot-ros-w-publisher) | Wheel angular velocity publisher. |
| | [4w-robot-ros-imu-gps](https://github.com/adrianmarino/4w-ros-robot-imu-gps) | IMU & GPS publisher. |
| | [4w-robot-ros-lidar](https://github.com/adrianmarino/4w-robot-ros-lidar) | LIDAR publisher. |
| **Libs** | [arduino-commons](https://github.com/adrianmarino/arduino-commons) | Common Arduino utilities. |
| **HW** | [4w-robot-ros-kicad](https://github.com/adrianmarino/4w-robot-ros-kicad) | PCB Designs. |

---

## 📦 Installation

Add the following to your `platformio.ini`. **Crucial:** `board_microros_transport` must be set to `wifi`.

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
board_microros_distro = humble
board_microros_transport = wifi
lib_deps =
  https://github.com/adrianmarino/arduino-ros
```

---

## 📚 API Reference

### Core & Lifecycle

#### **`RosNodeManager`**
Manages Wi-Fi connection, micro-ROS session initialization, and the node executor.
Uses `WiFiManager` to handle Wi-Fi connection (captive portal) and Agent IP/Port configuration.

*   **Include**: `#include "RosNodeManager.h"`
*   **Constructor**:
    ```cpp
    RosNodeManager(String nodeName, bool wifiEnergySavingMode = false, ...);
    ```
*   **Key Methods**:
    *   `setup()`: Connects to Wi-Fi (opens captive portal if needed) and Agent.
    *   `update(timeout_ns)`: Processes callbacks. Call in `loop()`.
    *   `isConnected()`: Pings the agent.

#### **`RosNodeManagerRestartHandler`**
Connection watchdog. Restarts the ESP32 if the Micro-ROS Agent is unreachable for a set duration.

*   **Include**: `#include "RosNodeManagerRestartHandler.h"`
*   **Usage**: call `update()` in the main loop instead of `nodeManager->update()`.

---

### Hardware Abstraction

#### **`BLDCMotor`**
Control logic for Brushless DC motors (PWM + Direction + Brake).

*   **Include**: `#include "BLDCMotor.h"`, `#include "BLDCMotorBuilder.h"`
*   **Builder**:
    ```cpp
    BLDCMotor *motor = BLDCMotorBuilder(pinPWM, pinDIR, pinBRAKE)
                        .setResolutionInBits(11)
                        .build();
    ```

---

### Communication

#### **Publishers**
Wrappers for standard messages.
*   `StringPublisher`, `IntPublisher`, `FloatPublisher` (takes `float`), `FloatArrayPublisher`.
*   **`DifferentialRobotOdometryPublisher`**: Publishes odometry data.

#### **`RosTwistSubscriber`**
Specialized subscriber for `geometry_msgs/msg/Twist` (velocity commands).
*   **Callback Signature**: `void onCmd(const void *msg)`

---

## 🚀 Usage Example

```cpp
#include <Arduino.h>
#include "ArduinoRos.h"

RosNodeManager *nodeManager;
RosTwistSubscriber *sub;
BLDCMotor *motor;
WToSignedPWMConverter *conv;

void onCmdVel(const void *msg) {
    float linearX = ((geometry_msgs__msg__Twist *)msg)->linear.x;
    int pwm = conv->wToSignedPWM(linearX / 0.05); // r=0.05m
    motor->setPwmSpeed(pwm);
}

void setup() {
    motor = new BLDCMotorBuilder(25, 26, 27).build();
    motor->setup();
    conv = new WToSignedPWMConverter(10.0, 12, 100);

    // Node Name: "bot_node"
    // Wi-Fi and Agent IP are configured via Captive Portal on first run.
    nodeManager = new RosNodeManager("bot_node"); 
    nodeManager->setup();

    sub = new RosTwistSubscriber(nodeManager->getNode(), nodeManager->getExecutor(), "cmd_vel", onCmdVel);
}

void loop() {
    if (!nodeManager->update()) ESP.restart();
}
```

## 🛠 Build

```bash
cd examples/basic_test
pio run
```
