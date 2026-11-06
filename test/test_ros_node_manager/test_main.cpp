#include <unity.h>

#include <Arduino.h>
#include <RosNodeManager.h>

void test_update_reports_a_failed_agent_ping() {
  arduino_mock::reset();
  microros_mock::ping_result = RCL_RET_ERROR;
  arduino_mock::now_ms = 10000;
  RosNodeManager manager("cutting_system_node");

  TEST_ASSERT_FALSE(manager.update());
}

void test_update_accepts_a_successful_agent_ping() {
  arduino_mock::reset();
  microros_mock::ping_result = RMW_RET_OK;
  arduino_mock::now_ms = 10000;
  RosNodeManager manager("cutting_system_node");

  TEST_ASSERT_TRUE(manager.update());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_update_reports_a_failed_agent_ping);
  RUN_TEST(test_update_accepts_a_successful_agent_ping);
  return UNITY_END();
}
