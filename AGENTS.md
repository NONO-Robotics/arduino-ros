# Project Instructions (AGENTS.md)

## 1. 🎯 Role & Ecosystem Context
*   **Subproject Role**: Common shared ROS 2 / micro-ROS publisher/subscriber wrapper library.
*   **Robot Variant**: Common — Indoor + Outdoor robots.
*   **Ecosystem Integration**: Standardizes ESP32 sensor publishing (IMU, GPS, Odometry) + velocity command (`Twist`) receiving. Shields drivers from raw micro-ROS client APIs.

## 2. 🛠️ Tech Stack & Build Workflow
*   **Framework**: PlatformIO / Arduino / micro-ROS (Humble) / ESP32.
*   **Language**: C++ (ISO).
*   **Build**: `pio run`

## 3. ⚙️ Core Interfaces & Components
*   **RosNodeManager**: Node lifecycle manager — handles Wi-Fi, serial configs, automatic hardware watchdog (restarts ESP32 if agent connection lost).
*   **Publishers**: `IMUPublisher`, `GPSPublisher`, `DifferentialRobotOdometryPublisher`, `FloatPublisher`, etc.
*   **Subscribers**: `RosTwistSubscriber` → `/cmd_vel` velocity streams.

## 4. 📜 Coding Standards & Conventions
*   **Language & Comments**: English + Doxygen (`/** @brief ... */`) for interfaces.
*   **Watchdog Execution**: Always call `nodeManager->update()` in main loop.
*   **Memory Safety**: Static allocation or pre-allocated arrays (e.g. `char frame_id_buffer[20]`) — no micro-ROS allocator memory leaks.

## 📜 Object-Oriented Programming & SOLID Standards
*   **SOLID Principles**: Strict C++ & Arduino SOLID practices:
    *   `S (SRP)`: Separate hardware communication, data parsing, ROS publishers.
    *   `O (OCP)`: Polymorphism/abstract interfaces (e.g. `DCMotor`, `IMUSensor`) — add new models without editing client logic.
    *   `L (LSP)`: Subclasses (e.g. `BLDCMotor`) fully substitutable for parent interface (`DCMotor`).
    *   `I (ISP)`: Lightweight cohesive interfaces: `Updatable`, `Drawable`.
    *   `D (DIP)`: Inject dependencies via references/pointers to abstract classes, not hardcoded instances.
*   **Embedded Design Patterns**:
    *   `Fluent Builder`: Configure hardware modules cleanly (e.g. `BLDCMotorBuilder`), no bloated constructors.
    *   `Strategy`: Decouple control algorithms (Mecanum vs Differential) from actuator drivers.
    *   `Observer / Callback`: Non-blocking events, function pointers/lambdas for async ROS subscription/polling.
