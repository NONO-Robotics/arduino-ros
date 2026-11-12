#pragma once

#include <cstdint>
#include <vector>

#include <rcl/rcl.h>

struct rclc_executor_t {
  int dummy = 0;
};

struct rclc_support_t {
  rcl_context_t context;
};

namespace rclc_node_manager_mock {
inline rcl_ret_t spin_result = RCL_RET_OK;
inline rcl_ret_t support_init_result = RCL_RET_OK;
inline rcl_ret_t node_init_result = RCL_RET_OK;
inline rcl_ret_t executor_init_result = RCL_RET_OK;

inline void reset() {
  spin_result = RCL_RET_OK;
  support_init_result = RCL_RET_OK;
  node_init_result = RCL_RET_OK;
  executor_init_result = RCL_RET_OK;
}
}

inline rcl_ret_t rclc_executor_spin_some(rclc_executor_t *, uint64_t) {
  return rclc_node_manager_mock::spin_result;
}

inline rcl_ret_t rclc_executor_init(rclc_executor_t *, rcl_context_t *, size_t,
                                     rcl_allocator_t *) {
  return rclc_node_manager_mock::executor_init_result;
}

inline rcl_ret_t rclc_support_init(rclc_support_t *, int, void *,
                                   rcl_allocator_t *) {
  return rclc_node_manager_mock::support_init_result;
}

inline rcl_ret_t rclc_node_init_default(rcl_node_t *, const char *, const char *,
                                         rclc_support_t *) {
  return rclc_node_manager_mock::node_init_result;
}

constexpr int ON_NEW_DATA = 0;

namespace rclc_mock {
struct SubscriptionRegistration {
  const void *message;
  void (*callback)(const void *);
  std::string topic;
};
inline std::vector<SubscriptionRegistration> registrations;

inline void reset() { registrations.clear(); }
}  // namespace rclc_mock

inline rcl_ret_t rclc_subscription_init_default(
    rcl_subscription_t *subscriber,
    rcl_node_t *node,
    const rosidl_message_type_support_t *type_support,
    const char *topic_name) {
  (void)node;
  (void)type_support;
  ++rcl_mock::subscription_inits;
  if (rcl_mock::subscription_init_fails) return RCL_RET_ERROR;
  subscriber->topic = topic_name;
  rcl_mock::last_topic = topic_name;
  return RCL_RET_OK;
}

inline rcl_ret_t rclc_executor_add_subscription(
    rclc_executor_t *executor,
    rcl_subscription_t *subscriber,
    const void *msg,
    void (*callback)(const void *),
    int ignored) {
  (void)executor;
  (void)ignored;
  ++rcl_mock::executor_adds;
  if (rcl_mock::executor_add_fails) return RCL_RET_ERROR;
  if (subscriber->topic == nullptr) return RCL_RET_ERROR;
  rclc_mock::registrations.push_back({msg, callback, subscriber->topic});
  return RCL_RET_OK;
}
