#include <unity.h>

#include <Arduino.h>

#include "Float32Subscriber.h"
#include "Logger.h"
#include "NativeTestCommon.h"

namespace {

rcl_node_t node;
rclc_executor_t executor;

int firstEvents = 0;
int secondEvents = 0;
float firstValue = 0.0F;

void onFirst(const std_msgs__msg__Float32 &message) {
  ++firstEvents;
  firstValue = message.data;
}

void onSecond(const std_msgs__msg__Float32 &) { ++secondEvents; }

}  // namespace

void test_constructor_registers_float32_subscription() {
  resetMocks();

  new Float32Subscriber(&node, &executor, "/cutting/speed", onFirst);

  TEST_ASSERT_EQUAL_INT(1, rcl_mock::subscription_inits);
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::executor_adds);
  TEST_ASSERT_EQUAL_STRING("/cutting/speed", rcl_mock::last_topic.c_str());
  TEST_ASSERT_EQUAL_STRING("std_msgs/msg/Float32", rcl_mock::last_type_support.c_str());
}

void test_float32_message_reaches_its_typed_callback() {
  resetMocks();
  firstEvents = 0;
  secondEvents = 0;
  firstValue = 0.0F;
  new Float32Subscriber(&node, &executor, "/first", onFirst);
  new Float32Subscriber(&node, &executor, "/second", onSecond);

  auto *firstMessage = static_cast<std_msgs__msg__Float32 *>(
      const_cast<void *>(rclc_mock::registrations[0].message));
  firstMessage->data = 42.5F;
  rclc_mock::registrations[0].callback(firstMessage);

  TEST_ASSERT_EQUAL_INT(1, firstEvents);
  TEST_ASSERT_FLOAT_WITHIN(0.0001F, 42.5F, firstValue);
  TEST_ASSERT_EQUAL_INT(0, secondEvents);
}

void test_null_message_is_ignored() {
  resetMocks();
  firstEvents = 0;
  new Float32Subscriber(&node, &executor, "/first", onFirst);

  rclc_mock::registrations[0].callback(nullptr);

  TEST_ASSERT_EQUAL_INT(0, firstEvents);
}

void test_subscriber_without_callback_is_ignored() {
  resetMocks();
  firstEvents = 0;
  new Float32Subscriber(&node, &executor, "/silent", nullptr);

  rclc_mock::registrations[0].callback(rclc_mock::registrations[0].message);

  TEST_ASSERT_EQUAL_INT(0, firstEvents);
}

void test_subscription_init_failure_is_reported_by_logger() {
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::subscription_init_fails = true;

  new Float32Subscriber(&node, &executor, "/bad", onFirst);

  TEST_ASSERT_NOT_NULL(strstr(
      Serial.output().c_str(), "[ERROR] Error to create subscriber: /bad"));
}

void test_executor_add_failure_is_reported_by_logger() {
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::executor_add_fails = true;

  new Float32Subscriber(&node, &executor, "/bad_exec", onFirst);

  TEST_ASSERT_NOT_NULL(strstr(
      Serial.output().c_str(), "[ERROR] Error subscribing to topic: /bad_exec"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_constructor_registers_float32_subscription);
  RUN_TEST(test_float32_message_reaches_its_typed_callback);
  RUN_TEST(test_null_message_is_ignored);
  RUN_TEST(test_subscriber_without_callback_is_ignored);
  RUN_TEST(test_subscription_init_failure_is_reported_by_logger);
  RUN_TEST(test_executor_add_failure_is_reported_by_logger);
  return UNITY_END();
}
