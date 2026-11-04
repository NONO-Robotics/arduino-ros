#include <unity.h>

#include <Arduino.h>

#include "BoolSubscriber.h"
#include "Logger.h"
#include "NativeTestCommon.h"

namespace {

rcl_node_t node;
rclc_executor_t executor;

int firstEvents = 0;
int secondEvents = 0;
bool firstValue = false;
bool secondValue = false;

void onFirst(const std_msgs__msg__Bool &message) {
  ++firstEvents;
  firstValue = message.data;
}

void onSecond(const std_msgs__msg__Bool &message) {
  ++secondEvents;
  secondValue = message.data;
}

}  // namespace

void test_constructor_registers_bool_subscription() {
  resetMocks();

  new BoolSubscriber(&node, &executor, "/cutting/on_off", onFirst);

  TEST_ASSERT_EQUAL_INT(1, rcl_mock::subscription_inits);
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::executor_adds);
  TEST_ASSERT_EQUAL_STRING("/cutting/on_off", rcl_mock::last_topic.c_str());
  TEST_ASSERT_EQUAL_STRING("std_msgs/msg/Bool", rcl_mock::last_type_support.c_str());
}

void test_bool_message_reaches_its_typed_callback() {
  resetMocks();
  firstEvents = 0;
  secondEvents = 0;
  firstValue = false;
  secondValue = false;
  new BoolSubscriber(&node, &executor, "/first", onFirst);
  new BoolSubscriber(&node, &executor, "/second", onSecond);

  auto *firstMessage = static_cast<std_msgs__msg__Bool *>(
      const_cast<void *>(rclc_mock::registrations[0].message));
  firstMessage->data = true;
  rclc_mock::registrations[0].callback(firstMessage);

  TEST_ASSERT_EQUAL_INT(1, firstEvents);
  TEST_ASSERT_TRUE(firstValue);
  TEST_ASSERT_EQUAL_INT(0, secondEvents);
  TEST_ASSERT_FALSE(secondValue);
}

void test_null_message_is_ignored() {
  resetMocks();
  firstEvents = 0;
  new BoolSubscriber(&node, &executor, "/first", onFirst);

  rclc_mock::registrations[0].callback(nullptr);

  TEST_ASSERT_EQUAL_INT(0, firstEvents);
}

void test_subscriber_without_callback_is_ignored() {
  resetMocks();
  firstEvents = 0;
  new BoolSubscriber(&node, &executor, "/silent", nullptr);

  rclc_mock::registrations[0].callback(
      rclc_mock::registrations[0].message);

  TEST_ASSERT_EQUAL_INT(0, firstEvents);
}

void test_subscription_init_failure_is_reported_by_logger() {
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::subscription_init_fails = true;

  new BoolSubscriber(&node, &executor, "/bad", onFirst);

  TEST_ASSERT_NOT_NULL(strstr(
      Serial.output().c_str(), "[ERROR] Error to create subscriber: /bad"));
}

void test_executor_add_failure_is_reported_by_logger() {
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::executor_add_fails = true;

  new BoolSubscriber(&node, &executor, "/bad_exec", onFirst);

  TEST_ASSERT_NOT_NULL(strstr(
      Serial.output().c_str(), "[ERROR] Error suscribing to topic: /bad_exec"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_constructor_registers_bool_subscription);
  RUN_TEST(test_bool_message_reaches_its_typed_callback);
  RUN_TEST(test_null_message_is_ignored);
  RUN_TEST(test_subscriber_without_callback_is_ignored);
  RUN_TEST(test_subscription_init_failure_is_reported_by_logger);
  RUN_TEST(test_executor_add_failure_is_reported_by_logger);
  return UNITY_END();
}
