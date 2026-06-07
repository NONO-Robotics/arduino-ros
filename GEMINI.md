# Project Instructions (GEMINI.md)

## 1. 🎯 Role & Ecosystem Context
*   **Subproject Role**: Common shared ROS 2 and micro-ROS publisher/subscriber wrapper library.
*   **Robot Variant**: Common to both Indoor and Outdoor robots.
*   **Ecosystem Integration**: Standardizes how ESP32 nodes publish spatial sensor data (IMU, GPS, Odometry) and receive velocity commands (`Twist`), shielding drivers from raw micro-ROS client APIs.

## 2. 🛠️ Tech Stack & Build Workflow
*   **Framework**: PlatformIO / Arduino / micro-ROS (Humble) / ESP32.
*   **Language**: C++ (Standard ISO).
*   **Build Command**: `pio run`

## 3. ⚙️ Core Interfaces & Components
*   **RosNodeManager**: Monolith node lifecycle manager. It handles Wi-Fi connection, serial configurations, and implements an automatic hardware watchdog (restarts the ESP32 if connection to the agent is lost).
*   **Publishers**: Wrappers for `IMUPublisher`, `GPSPublisher`, `DifferentialRobotOdometryPublisher`, `FloatPublisher`, etc.
*   **Subscribers**: `RosTwistSubscriber` to process `/cmd_vel` velocity streams.

## 4. 📜 Coding Standards & Conventions
*   **Language & Comments**: Always use **English** and Doxygen formatting (`/** @brief ... */`) for interfaces.
*   **Watchdog Execution**: Always invoke `nodeManager->update()` in the main loop to keep the watchdog active.
*   **Memory Safety**: Frame IDs and string buffers must use static allocation or pre-allocated arrays (e.g. `char frame_id_buffer[20]`) to prevent memory leaks in the micro-ROS allocator.

## 📜 Object-Oriented Programming & SOLID Standards
*   **SOLID Principles**: Strictly follow SOLID practices adapted for C++ & Arduino:
    *   `S (Single Responsibility)`: Separate hardware communication, data parsing, and ROS publishers into different classes.
    *   `O (Open/Closed)`: Favor polymorphism and abstract interfaces (e.g. abstract classes for DCMotor, IMUSensor) to allow adding new models without editing client logic.
    *   `L (Liskov Substitution)`: Subclasses (e.g. BLDCMotor) must be fully substitutable for their parent interface (DCMotor).
    *   `I (Interface Segregation)`: Maintain lightweight, cohesive interfaces focused on distinct behaviors (e.g. Updatable, Drawable).
    *   `D (Dependency Inversion)`: Inject dependencies via references or pointers to abstract classes (Dependency Injection) rather than hardcoding concrete instances.
*   **Embedded Design Patterns**: Use microcontroller-optimized design patterns:
    *   `Fluent Builder`: To cleanly configure and initialize hardware modules (e.g. BLDCMotorBuilder) without bloated constructors.
    *   `Strategy`: Decouple control algorithms (e.g. Mecanum vs Differential Kinematics) from physical actuator drivers.
    *   `Observer / Callback`: Use non-blocking events and function pointers/lambdas for asynchronous ROS subscription and polling tasks.
