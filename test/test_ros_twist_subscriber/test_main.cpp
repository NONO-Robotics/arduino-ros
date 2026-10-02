#include <unity.h>

#include <Arduino.h>

#include "Logger.h"
#include "NativeTestCommon.h"
#include "RosTwistSubscriber.h"

static rcl_node_t node;
static rclc_executor_t executor;

static int first_events = 0;
static int second_events = 0;
static int null_event_events = 0;

static void onFirst(geometry_msgs__msg__Twist *) { ++first_events; }
static void onSecond(geometry_msgs__msg__Twist *) { ++second_events; }
static void onNullEvent(geometry_msgs__msg__Twist *) { ++null_event_events; }

void test_constructor_registers_subscription_with_executor() {
  // Prepare
  resetMocks();

  // Perform
  new RosTwistSubscriber(&node, &executor, "/cmd_vel", onFirst);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::subscription_inits);
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::executor_adds);
  TEST_ASSERT_EQUAL_STRING("/cmd_vel", rcl_mock::last_topic.c_str());
  TEST_ASSERT_EQUAL_STRING("geometry_msgs/msg/Twist",
                           rcl_mock::last_type_support.c_str());
  TEST_ASSERT_EQUAL_UINT(1, rclc_mock::registrations.size());
}

void test_message_dispatch_reaches_matching_subscriber_only() {
  // Prepare
  resetMocks();
  new RosTwistSubscriber(&node, &executor, "/cmd_vel", onFirst);
  new RosTwistSubscriber(&node, &executor, "/cmd_vel_other", onSecond);
  first_events = 0;
  second_events = 0;

  // Perform
  rclc_mock::registrations[0].callback(
      rclc_mock::registrations[0].message);
  rclc_mock::registrations[1].callback(
      rclc_mock::registrations[1].message);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, first_events);
  TEST_ASSERT_EQUAL_INT(1, second_events);
}

void test_null_message_is_ignored() {
  // Prepare
  resetMocks();
  new RosTwistSubscriber(&node, &executor, "/cmd_vel", onFirst);
  first_events = 0;

  // Perform
  rclc_mock::registrations[0].callback(nullptr);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, first_events);
}

void test_subscriber_without_event_callback_is_skipped() {
  // Prepare
  resetMocks();
  new RosTwistSubscriber(&node, &executor, "/silent", nullptr);
  new RosTwistSubscriber(&node, &executor, "/cmd_vel", onFirst);
  first_events = 0;
  null_event_events = 0;

  // Perform
  rclc_mock::registrations[1].callback(
      rclc_mock::registrations[1].message);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, first_events);

  // Perform — message belonging to the callback-less subscriber
  rclc_mock::registrations[0].callback(
      rclc_mock::registrations[0].message);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, first_events);
  TEST_ASSERT_EQUAL_INT(0, null_event_events);
}

void test_subscription_init_failure_is_reported_by_logger() {
  // Prepare
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::subscription_init_fails = true;

  // Perform
  new RosTwistSubscriber(&node, &executor, "/bad", onFirst);

  // Asserts
  TEST_ASSERT_NOT_NULL(strstr(
      Serial.output().c_str(),
      "[ERROR] Error to create subscriber: /bad"));
}

void test_executor_add_failure_is_reported_by_logger() {
  // Prepare
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::executor_add_fails = true;

  // Perform
  new RosTwistSubscriber(&node, &executor, "/bad_exec", onFirst);

  // Asserts
  TEST_ASSERT_NOT_NULL(strstr(
      Serial.output().c_str(),
      "[ERROR] Error suscribing to topic: /bad_exec"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_constructor_registers_subscription_with_executor);
  RUN_TEST(test_message_dispatch_reaches_matching_subscriber_only);
  RUN_TEST(test_null_message_is_ignored);
  RUN_TEST(test_subscriber_without_event_callback_is_skipped);
  RUN_TEST(test_subscription_init_failure_is_reported_by_logger);
  RUN_TEST(test_executor_add_failure_is_reported_by_logger);
  return UNITY_END();
}
