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
