#pragma once

#include <rcl/rcl.h>

inline rcl_ret_t rclc_publisher_init_default(
    rcl_publisher_t *publisher,
    rcl_node_t *node,
    const rosidl_message_type_support_t *type_support,
    const char *topic_name) {
  (void)node;
  ++rcl_mock::publisher_default_inits;
  if (rcl_mock::publisher_init_fails) return RCL_RET_ERROR;
  publisher->type_support = type_support;
  publisher->topic = topic_name;
  rcl_mock::last_topic = topic_name;
  return RCL_RET_OK;
}

inline rcl_ret_t rclc_publisher_init_best_effort(
    rcl_publisher_t *publisher,
    rcl_node_t *node,
    const rosidl_message_type_support_t *type_support,
    const char *topic_name) {
  (void)node;
  ++rcl_mock::publisher_best_effort_inits;
  if (rcl_mock::publisher_init_fails) return RCL_RET_ERROR;
  publisher->type_support = type_support;
  publisher->topic = topic_name;
  rcl_mock::last_topic = topic_name;
  return RCL_RET_OK;
}
