# Arduino-ROS Library

This library provides a robust framework for integrating micro-ROS on ESP32 microcontrollers using PlatformIO. It facilitates node lifecycle management, Wi-Fi connection, message publishing/subscription, and specific hardware control (BLDC motors and encoders).


## Related projects

* [4w-ros-robot](https://github.com/adrianmarino/4w-ros-robot)
  * **Navigation**
    * [4w-robot-ros-ws](https://github.com/adrianmarino/4w-robot-ros-ws): Autonomous/manual navigation control project.
    * [4w-robot-ros-movement](https://github.com/adrianmarino/4w-robot-ros-movement.git): Movement controller firmware.
  * **Sensor data publisher firmware**
    * [4w-robot-ros-w-publisher](https://github.com/adrianmarino/4w-robot-ros-w-publisher): Wheels angular velocity sensors publisher firmware.
    * [4w-robot-ros-imu-gps](https://github.com/adrianmarino/4w-ros-robot-imu-gps): IMU, GPS sensors publisher firmware.
    * [4w-robot-ros-lidar](https://github.com/adrianmarino/4w-robot-ros-lidar): LIDAR sensor publisher firmware.
  * **Libraries**
    * [ardino-ros](https://github.com/adrianmarino/arduino-ros): ROS common library.
    * [ardino-commons](https://github.com/adrianmarino/arduino-commons): Arduino common library.
  * **Design**
    * [4w-robot-ros-kicad](https://github.com/adrianmarino/4w-robot-ros-kicad) PCB's Design.
    * [Solidworks Model](https://drive.google.com/drive/folders/1mQg-BSRZyyYhnBoig6Qm0Zf43U8bTAA7?usp=sharing): 3D model design.


---

## Installation and Dependencies

To use this library, ensure your `platformio.ini` file includes the following configurations and dependencies. It is critical to configure the micro-ROS transport as `wifi`.

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

## Core Classes

### RosNodeManager

Manages the Wi-Fi connection, the initialization of the session with the micro-ROS Agent, and the node executor.

**Include**: `#include "RosNodeManager.h"`

**Constructor**
```c++
RosNodeManager(
    String nodeName,            // ROS 2 Node Name
    String wifi_ssid,           // Wi-Fi SSID
    String wifi_pass,           // Wi-Fi Password
    String agent_ip,            // IP Address of the PC running the Agent
    uint16_t agent_port = 8888, // Agent Port (Default: 8888)
    bool wifiEnergySavingMode = false, // Power saving mode (false recommended for robots)
    wifi_power_t wifi_power = WIFI_POWER_20_5dBm, // TX Power
    bool syncTime = true        // Sync clock upon connection
);
```

**Main Methods**

* `RosNodeManager* setup()`: Initiates Wi-Fi connection and connects to the agent. Must be called in setup().
* `bool update(uint64_t timeout_ns)`: Maintains communication and processes callbacks. Must be called in loop().
* `bool isConnected(int timeout_ms, uint8_t attempts)`: Checks if the agent responds to pings.
* `rcl_node_t* getNode()`: Returns the raw pointer to the node (needed for manually creating entities).
* `rclc_executor_t* getExecutor()`: Returns the executor (needed for subscribers).



## RosNodeManagerRestartHandler

A "Watchdog" monitor for the connection. If the connection to the agent is lost for a specified time, it restarts the microcontroller.

**Includes**
* `#include "RosNodeManagerRestartHandler.h"`

**Constructor**

```c++
RosNodeManagerRestartHandler(
    RosNodeManager *nodeManager,
    int checkConnectionIntervalMs = 10000, // Check interval
    int timeout_ms = 5000                  // Ping timeout
);
```

**Methods**

* `void update()`: Executes the verification. Replaces the direct call to `nodeManager->update()` in the main loop.

## Communication (Publishers & Subscribers)

### MicroRosPublisher (Base)

Base class wrapping `rcl_publisher_t`. It is recommended to use its static factory methods.

**Includes**
* `#include "MicroRosPublisher.h"`

**Factory Methods (Static)**

* `static MicroRosPublisher* createString(...)`
* `static MicroRosPublisher* createInt(...)`
* `static MicroRosPublisher* createFloat(...)`
* `static MicroRosPublisher* createFloatArray(...)`

**Common Parameters**

* `(rcl_node_t *node, String topic_name, int delayMillis, bool reliable)`

### Typed Publishers

Specific wrappers to facilitate data transmission.

**Includes**
* `#include "StringPublisher.h"`
* `#include "IntPublisher.h"`
* `#include "FloatPublisher.h"`
* `#include "FloatArrayPublisher.h"`

**Classes**
* **StringPublisher**
    * `void publish(const String &data)`
* **IntPublisher**
    * `void publish(int value)`
* **FloatPublisher**
    * `void publish(float value)`
* **FloatArrayPublisher**
    * Constructor: `FloatArrayPublisher(MicroRosPublisher* pub, size_t length)`
    * `void publish(float *data_array)`
    
### RosTwistSubscriber

Specialized subscriber for `geometry_msgs/msg/Twist` messages (velocity).

**Include**: `#include "RosTwistSubscriber.h"`

**Constructor**

```c++
RosTwistSubscriber(
    rcl_node_t *node,
    rclc_executor_t *executor,
    String topic_name,
    void (*onReceiveMessage)(const void *) // Callback Function
);
```

**Callback**

The callback function must have the signature `void name(const void * msg)`. Inside, cast `msg` to `const geometry_msgs__msg__Twist *`.

## Hardware: Motors and Sensors

### BLDCMotor

Controller for Brushless DC motors using 3 pins (PWM, Direction, Brake).

**Includes**:
* `#include "BLDCMotor.h"`
* `#include "BLDCMotorBuilder.h"`

**Construction (Builder)**

```c++
BLDCMotor *motor = BLDCMotorBuilder(pinPWM, pinDIR, pinBRAKE)
                    .setChannel(0)           // ESP32 PWM Channel (0-15)
                    .setResolutionInBits(11) // PWM Resolution (e.g., 2047 steps)
                    .setFrequency(20000)     // Frequency Hz
                    .invertDirection()       // Optional: invert rotation
                    .build();
```
**Methods**

* `setup()`: Configures pins and LEDC channel.
* `setPwmSpeed(int speed)`: Sets speed (Range +/- depends on resolution).
* `brake()`: Activates the physical brake.
* `releaseBrake()`: Releases the brake.

### MagneticEncoder (AS5600)

Reads the position of the AS5600 magnetic sensor via I2C and calculates angular velocity ($w$).

**Includes**
* `#include "MagneticEncoder.h"`
* `#include "MagneticEncoderBuilder.h"`

**Construction (Builder)**
```c++
MagneticEncoder *encoder = MagneticEncoderBuilder()
                           .setCallback(onEncoderUpdate) // Function void(short ch, int step, float w)
                           .setSampleInterval(50)        // ms between readings
                           .setI2CAddress(0x36)
                           .setAlpha(0.8)                // Low pass filter coefficient (0.0 - 1.0)
                           .build();
```
**Methods**
* `begin()`: Initializes the sensor. Returns false if it fails.
* `update()`: Reads the sensor and calculates velocity. Call frequently in loop.
* `float getW()`: Returns the last calculated angular velocity in rad/s.

## Kinematics and Utilities

### FWKinematics

Inverse kinematics for 4-wheeled robots (Mecanum/Omni).

**Include**: `#include "FWKinematics.h"`

**Constructor**

```c++
FWKinematics(
    float l, // Longitudinal distance between wheels
    float w, // Transverse distance between wheels
    float r  // Wheel radius
);
```
**Methods**

* `twistTofwAngularSpeed(Twist *msg, FWAngularSpeed *out)`: Converts a Twist message into angular velocities for the 4 wheels.


### VelocityConverter

Converts angular velocity (rad/s) to PWM value.

**Include**: `#include "VelocityConverter.h"`

**Constructor**

```c++
VelocityConverter(
    float maxWInRadSeg,      // Max physical angular velocity of the motor
    int pwmResolutionInBits, // PWM resolution used
    int minPwm               // Minimum deadzone
);
```
**Methods**

* `int wToSignedPWM(float w)`: Returns the corresponding PWM value.


## Complete Example (`main.cpp`)

This example creates a node, subscribes to `/cmd_vel`, and controls a motor based on linear X velocity.

```c++
#include <Arduino.h>
#include "ArduinoRos.h"
#include "BLDCMotorBuilder.h"
#include "VelocityConverter.h"

// --- Configuration ---
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_PASSWORD"
#define AGENT_IP "192.168.1.100"
#define NODE_NAME "esp32_bot"

// --- Global Objects ---
RosNodeManager *nodeManager;

RosTwistSubscriber *cmdSubscriber;

BLDCMotor *motorLeft;

VelocityConverter *converter;

// --- Subscription Callback ---
void onCmdVel(const void *msg) {
    // Get desired linear velocity (m/s)
    float linearX = msg->linear.x;
    
    // Convert m/s to rad/s (assuming wheel radius 0.05m)
    float targetW = linearX / 0.05;

    // Convert rad/s to PWM
    int pwm = converter->wToSignedPWM(targetW);

    // Move the motor
    motorLeft->setPwmSpeed(pwm);
}

void setup() {
    Serial.begin(115200);

    // 1. Configure Hardware
    motorLeft = new BLDCMotorBuilder(25, 26, 27)
                .setResolutionInBits(12)
                .build();
    motorLeft->setup();

    converter = new VelocityConverter(10.0, 12, 100); // Max 10 rad/s, 12 bits, min PWM 100

    // 2. Configure ROS
    nodeManager = new RosNodeManager(NODE_NAME, WIFI_SSID, WIFI_PASS, AGENT_IP);
    nodeManager->setup();

    // 3. Create Subscriber
    cmdSubscriber = new RosTwistSubscriber(
        nodeManager->getNode(),
        nodeManager->getExecutor(),
        "cmd_vel",
        onCmdVel
    );
}

void loop() {
    // Maintains connection and processes messages
    if (!nodeManager->update()) {
        // Simple reconnection or restart logic
        ESP.restart();
    }
}
```



## Build

```bash
cd examples/basic_test
pio run
```
