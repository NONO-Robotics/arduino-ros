/**
 * @file RosMessage.h
 * @brief Helper functions to create ROS messages.
 */
#pragma once
#include "RosUtils.h"
#include <std_msgs/msg/float32.h>
#include <std_msgs/msg/float32_multi_array.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/string.h>
#include <geometry_msgs/msg/vector3_stamped.h>

/**
 * @brief Create a Float32 message.
 * @return Pointer to the created std_msgs__msg__Float32 message.
 */
std_msgs__msg__Float32 *createFloatMessage();

/**
 * @brief Create an Int32 message.
 * @return Pointer to the created std_msgs__msg__Int32 message.
 */
std_msgs__msg__Int32 *createIntMessage();

/**
 * @brief Create a Float32MultiArray message.
 * @param length Length of the array.
 * @return Pointer to the created std_msgs__msg__Float32MultiArray message.
 */
std_msgs__msg__Float32MultiArray *createFloatArrayMessage(size_t length);

/**
 * @brief Create a geometry_msgs__msg__Vector3Stamped message.
 * @param frameId frame id string.
 * @return Pointer to the created geometry_msgs__msg__Vector3Stamped message.
 */
geometry_msgs__msg__Vector3Stamped *createVector3StampedMessage(String frameId);
