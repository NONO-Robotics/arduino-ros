#pragma once

#include <map>
#include <string>

using rcl_ret_t = int;
constexpr rcl_ret_t RCL_RET_OK = 0;
constexpr rcl_ret_t RCL_RET_ERROR = 1;

struct rosidl_message_type_support_t {
  const char *name = nullptr;
};

struct rcl_node_t {
  int dummy = 0;
};

struct rcl_publisher_t {
  const rosidl_message_type_support_t *type_support = nullptr;
  const char *topic = nullptr;
};

struct rcl_subscription_t {
  const char *topic = nullptr;
};

namespace rcl_mock {
inline bool publisher_init_fails = false;
inline bool publish_fails = false;
inline bool subscription_init_fails = false;
inline bool executor_add_fails = false;
inline int publisher_default_inits = 0;
inline int publisher_best_effort_inits = 0;
inline int publishes = 0;
inline int subscription_inits = 0;
inline int executor_adds = 0;
inline std::string last_topic;
inline std::string last_type_support;
inline const void *last_message = nullptr;

inline void reset() {
  publisher_init_fails = false;
  publish_fails = false;
  subscription_init_fails = false;
  executor_add_fails = false;
  publisher_default_inits = 0;
  publisher_best_effort_inits = 0;
  publishes = 0;
  subscription_inits = 0;
  executor_adds = 0;
  last_topic.clear();
  last_type_support.clear();
  last_message = nullptr;
}
}  // namespace rcl_mock

inline const rosidl_message_type_support_t *rosidl_fake_typesupport(
    const char *name) {
  static std::map<std::string, rosidl_message_type_support_t> registry;
  auto &entry = registry[name];
  entry.name = name;
  rcl_mock::last_type_support = name;
  return &entry;
}

#define ROSIDL_GET_MSG_TYPE_SUPPORT(pkg, sub, name) \
  rosidl_fake_typesupport(#pkg "/" #sub "/" #name)

inline rcl_ret_t rcl_publish(const rcl_publisher_t *publisher,
                             const void *ros_message,
                             const void *allocation) {
  (void)publisher;
  (void)allocation;
  ++rcl_mock::publishes;
  rcl_mock::last_message = ros_message;
  return rcl_mock::publish_fails ? RCL_RET_ERROR : RCL_RET_OK;
}
