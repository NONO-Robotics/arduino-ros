#pragma once

#include <rcl/rcl.h>

constexpr rcl_ret_t RMW_RET_OK = RCL_RET_OK;

namespace microros_mock {
inline rcl_ret_t ping_result = RMW_RET_OK;
}

inline rcl_ret_t rmw_uros_ping_agent(int, int) {
  return microros_mock::ping_result;
}

inline rcl_ret_t rmw_uros_sync_session(int) { return RMW_RET_OK; }
inline bool rmw_uros_epoch_synchronized() { return true; }
inline int64_t rmw_uros_epoch_millis() { return 0; }
