# Shared micro-ROS library

- Shared Indoor/Outdoor ESP32 PlatformIO/Arduino/micro-ROS Humble library; build `pio run`.
- `RosNodeManager` manages connection lifecycle: first connect times out after 30 s and restarts ESP32; lost-agent watchdog restarts ESP32.
- Publishers include IMU, GPS, odometry, and floats; `RosTwistSubscriber` receives `/cmd_vel`.
- Call `nodeManager->update()` every loop. Use English Doxygen and static/preallocated memory; avoid micro-ROS allocator leaks.


## Pull requests only

- Every change must reach `main` through a Pull Request. This is the only permitted path: no direct pushes, no other merge route.
- Approval is always manual: open the PR, request the user's review, and stop.
- Never merge a branch into `main`.
- Never auto-approve a Pull Request.
