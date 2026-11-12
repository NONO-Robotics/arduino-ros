#include <unity.h>

#include <Arduino.h>
#include <RosNodeManager.h>

void resetNodeManagerMocks() {
  arduino_mock::reset();
  rclc_node_manager_mock::reset();
  microros_mock::ping_result = RMW_RET_OK;
}

void test_update_reports_a_failed_agent_ping() {
  resetNodeManagerMocks();
  microros_mock::ping_result = RCL_RET_ERROR;
  arduino_mock::now_ms = 10000;
  RosNodeManager manager("cutting_system_node");

  TEST_ASSERT_FALSE(manager.update());
}

void test_update_accepts_a_successful_agent_ping() {
  resetNodeManagerMocks();
  microros_mock::ping_result = RMW_RET_OK;
  arduino_mock::now_ms = 10000;
  RosNodeManager manager("cutting_system_node");

  TEST_ASSERT_TRUE(manager.update());
}

void test_update_skips_ping_before_interval() {
  resetNodeManagerMocks();
  RosNodeManager manager("node", true, 100, 5000);

  TEST_ASSERT_TRUE(manager.update());
}

void test_update_reports_executor_failure() {
  resetNodeManagerMocks();
  rclc_node_manager_mock::spin_result = RCL_RET_ERROR;
  RosNodeManager manager("node", true, 100, 5000);

  TEST_ASSERT_FALSE(manager.update());
}

void test_update_reaches_maximum_failed_pings() {
  resetNodeManagerMocks();
  microros_mock::ping_result = RCL_RET_ERROR;
  RosNodeManager manager("node", true, 1, 5000);

  for (unsigned long time = 1; time <= 5; ++time) {
    arduino_mock::now_ms = time;
    TEST_ASSERT_FALSE(manager.update());
  }
}

void test_setup_returns_initialized_handles() {
  resetNodeManagerMocks();
  RosNodeManager manager("node");

  TEST_ASSERT_EQUAL_PTR(&manager, manager.setup());
  TEST_ASSERT_NOT_NULL(manager.getSupport());
  TEST_ASSERT_NOT_NULL(manager.getNode());
  TEST_ASSERT_NOT_NULL(manager.getAllocator());
  TEST_ASSERT_NOT_NULL(manager.getExecutor());
}

void test_setup_handles_each_initialization_failure() {
  resetNodeManagerMocks();
  rclc_node_manager_mock::support_init_result = RCL_RET_ERROR;
  RosNodeManager supportManager("support");
  TEST_ASSERT_EQUAL_PTR(&supportManager, supportManager.setup());

  resetNodeManagerMocks();
  rclc_node_manager_mock::node_init_result = RCL_RET_ERROR;
  RosNodeManager nodeManager("node");
  TEST_ASSERT_EQUAL_PTR(&nodeManager, nodeManager.setup());

  resetNodeManagerMocks();
  rclc_node_manager_mock::executor_init_result = RCL_RET_ERROR;
  RosNodeManager executorManager("executor");
  TEST_ASSERT_EQUAL_PTR(&executorManager, executorManager.setup());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_update_reports_a_failed_agent_ping);
  RUN_TEST(test_update_accepts_a_successful_agent_ping);
  RUN_TEST(test_update_skips_ping_before_interval);
  RUN_TEST(test_update_reports_executor_failure);
  RUN_TEST(test_update_reaches_maximum_failed_pings);
  RUN_TEST(test_setup_returns_initialized_handles);
  RUN_TEST(test_setup_handles_each_initialization_failure);
  return UNITY_END();
}
