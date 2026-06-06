<div align="center">
  <img src="https://github.com/adrianmarino/4w-ros-robot/blob/main/images/indoor-preview2.jpg" alt="Arduino ROS Robot Logo"/>
  
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

This project is part of the **4w-ros-robot** family:

> 💡 **Naming Convention Tip:**
> * All subrepositories that **do not** have the prefix `outdoor` are used for the **Indoor Robot** version (except for shared utility libraries like `arduino-commons` and `arduino-ros`).
> * Subrepositories specifically belonging to the **Outdoor Robot** are prefixed with `outdoor` (with the exception of `4w-robot-cutting-control` which is specific to the grass-cutting system).

* [4w-ros-robot](https://github.com/adrianmarino/4w-ros-robot)
  * **Sensor data publisher firmware**
      * [4w-robot-ros-lidar](https://github.com/adrianmarino/4w-robot-ros-lidar): LIDAR sensor publisher firmware.
  * **Libraries**
    * [arduino-ros](https://github.com/adrianmarino/arduino-ros): ROS common library.
    * [arduino-commons](https://github.com/adrianmarino/arduino-commons): Arduino common library.
  * **Design**
    * [4w-robot-ros-kicad](https://github.com/adrianmarino/4w-robot-ros-kicad) PCB's Design.
    * [Solidworks Model](https://drive.google.com/drive/folders/1mQg-BSRZyyYhnBoig6Qm0Zf43U8bTAA7?usp=sharing): 3D model design.
  * **Indoor**
    * **Navigation**
      * [4w-robot-ros-ws](https://github.com/adrianmarino/4w-robot-ros-ws): Autonomous/manual navigation control project.
      * [4w-robot-ros-movement](https://github.com/adrianmarino/4w-robot-ros-movement.git): Movement controller firmware.
    * **Sensor data publisher firmware**
      * [4w-robot-ros-w-publisher](https://github.com/adrianmarino/4w-robot-ros-w-publisher): Wheels angular velocity sensors publisher firmware.
      * [4w-robot-ros-imu-gps](https://github.com/adrianmarino/4w-ros-robot-imu-gps): IMU, GPS sensors publisher firmware.
  * **Outdoor**
    * [4w-robot-cutting-control](https://github.com/adrianmarino/4w-robot-cutting-control): Automatic cutting motor controller.
    * **Navigation**
      * [4w-outdoor-robot-ros-ws](https://github.com/adrianmarino/4w-outdoor-robot-ros-ws): Autonomous/manual navigation control project.
      * [4w-outdoor-robot-ros-movement](https://github.com/adrianmarino/4w-outdoor-robot-ros-movement.git): Outdoor Movement controller firmware.
    * **Sensor data publisher firmware**
      * [4w-outdoor-robot-ros-w-publisher](https://github.com/adrianmarino/4w-outdoor-robot-ros-w-publisher): Outdoor wheels angular velocity sensors publisher firmware.
      * [4w-outdoor-robot-ros-imu-gps](https://github.com/adrianmarino/4w-outdoor-ros-robot-imu-gps): IMU, GPS sensors publisher firmware.

## 📦 Installation

Add the library to your `platformio.ini`. **Note:** You must configure the micro-ROS transport. See [Core & Lifecycle](#core--lifecycle) for the exact configuration needed for Wi-Fi or Serial connections.

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
board_microros_distro = humble
lib_deps =
  https://github.com/adrianmarino/arduino-ros
```

---

## 📚 API Reference

### Core & Lifecycle

#### **`RosNodeManager`**
Manages micro-ROS session initialization, the node executor, and transport connection.
It supports both **Wi-Fi** (via Captive Portal) and **Serial (USB)** connections. The connection mode is transparently selected using the `USE_WIFI_TRANSPORT` build flag.

**1. Wi-Fi Connection (Default/Standard)**
Uses `WiFiManager` to handle Wi-Fi connection and Agent IP/Port configuration.
*   **platformio.ini**:
    ```ini
    board_microros_transport = wifi
    build_flags = 
        -DUSE_WIFI_TRANSPORT
    ```
*   **Initialization**:
    ```cpp
    #include "ArduinoRos.h"
    RosNodeManager *nodeManager;

    void setup() {
        // Initializes Wi-Fi portal and connects to the micro-ROS agent
        nodeManager = (new RosNodeManager("bot_node"))->setup();
    }
    ```

**2. Serial (USB) Connection**
Used for wired communication (e.g., outdoor robots).
*   **platformio.ini**:
    ```ini
    board_microros_transport = serial
    ; Do NOT define USE_WIFI_TRANSPORT
    ```
*   **Initialization**:
    ```cpp
    #include "ArduinoRos.h"
    RosNodeManager *nodeManager;

    void setup() {
        // Initializes Serial connection to the micro-ROS agent
        nodeManager = (new RosNodeManager("bot_node"))->setup();
    }
    ```

*   **Key Methods**:
    *   `setup()`: Connects to Wi-Fi (if enabled) and initializes Agent session.
    *   `update(timeout_ns)`: Processes callbacks and monitors the connection status. If the connection is lost, it automatically restarts the ESP32. Call in `loop()`.
    *   `isConnected()`: Pings the agent.

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
*   **`DifferentialRobotOdometryPublisher`**: Publishes odometry data (`x`, `y`, `theta`).
    *   **Usage Context**: Essential in the wheel-publisher firmware. It computes differential drive odometry and sends it to the ROS 2 environment, providing necessary data for the robot's TF (Transform) tree and navigation algorithms.
*   **`IMUPublisher` & `GPSPublisher`**: Publish `sensor_msgs/msg/Imu` and `sensor_msgs/msg/NavSatFix`.
    *   **Usage Context**: Used in the dedicated sensor node to broadcast real-time state data. This data is typically consumed by `robot_localization` packages (EKF/UKF) to fuse with odometry for robust and accurate global navigation.

#### **`RosTwistSubscriber`**
Specialized subscriber for velocity commands (`geometry_msgs/msg/Twist`).
*   **Usage Context**: Primarily used in the robot's movement nodes. It receives teleoperation commands (from a joystick) or autonomous velocity commands (from Nav2), which the traction controllers then translate into wheel speeds.
*   **Callback Signature**: `void onCmd(const void *msg)`

---

## 💡 Class Examples

### 1. Basic Publishers (String, Int, Float)
```cpp
#include "ArduinoRos.h"

StringPublisher *strPub;
IntPublisher *intPub;

void setup() {
    // Assuming nodeManager is already set up...
    strPub = new StringPublisher(MicroRosPublisher::createString(nodeManager->getNode(), "status_topic"));
    intPub = new IntPublisher(MicroRosPublisher::createInt(nodeManager->getNode(), "battery_level"));
}

void loop() {
    strPub->publish("Robot is running smoothly");
    intPub->publish(85);
    delay(1000);
}
```

### 2. Sensor Publishers (IMU & GPS)
```cpp
#include "ArduinoRos.h"

IMUPublisher *imuPub;
GPSPublisher *gpsPub;

void setup() {
    imuPub = new IMUPublisher(nodeManager->getNode(), nodeManager->getAllocator(), "/imu/data", "imu_link");
    gpsPub = new GPSPublisher(nodeManager->getNode(), nodeManager->getAllocator(), "/gps/fix", "gps_link");
}

void loop() {
    IMUData imuData = { /* populate with sensor data */ };
    imuPub->publish(&imuData);
    
    GPSData gpsData = { /* populate with sensor data */ };
    gpsPub->publish(&gpsData);
    delay(100);
}
```

### 3. Differential Robot Odometry Publisher
```cpp
#include "ArduinoRos.h"

DifferentialRobotOdometryPublisher *odomPub;

void setup() {
    odomPub = new DifferentialRobotOdometryPublisher(nodeManager->getNode(), "odom", "base_link");
}

void loop() {
    DifferentialRobotOdometry odomData;
    odomData.x = 1.5;
    odomData.y = 2.0;
    odomData.theta = 0.5;
    odomPub->publish(odomData);
    delay(50);
}
```

### 4. Twist Subscriber
```cpp
#include "ArduinoRos.h"

RosTwistSubscriber *twistSub;

void onCmdVel(const void *msg) {
    geometry_msgs__msg__Twist *twist = (geometry_msgs__msg__Twist *)msg;
    // Use twist->linear.x and twist->angular.z to control the robot
}

void setup() {
    twistSub = new RosTwistSubscriber(nodeManager->getNode(), nodeManager->getExecutor(), "cmd_vel", onCmdVel);
}
```

### 5. Utilities (Logger & Time Sync)
```cpp
#include "ArduinoRos.h"

void setup() {
    logger.begin(115200, INFO, OUTPUT_SERIAL);
    logger.info("Initializing node...");
    
    // Sync time with agent
    if (MicroRosTimeUtils::syncSessionWithRetry(500, 10)) {
        logger.info("Time synchronized!");
    } else {
        logger.warn("Time sync failed.");
    }
}
```

---

## 🚀 Full Integration Example

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
