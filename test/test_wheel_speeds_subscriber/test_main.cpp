#include <unity.h>

#include <Arduino.h>
#include <cmath>

#include "Logger.h"
#include "NativeTestCommon.h"
#include "WheelSpeedsSubscriber.h"

namespace {

rcl_node_t node;
rclc_executor_t executor;

int speed_events = 0;
int fault_events = 0;
float last_average_left = 0.0f;
float last_fl = 0.0f;
float last_br = 0.0f;

void onSpeed(const WheelSpeeds &wheelSpeeds) {
  ++speed_events;
  last_average_left = wheelSpeeds.getAverageLeftWInRad();
  last_fl = wheelSpeeds.getFlWInRad();
  last_br = wheelSpeeds.getBrWInRad();
}

void onFault() { ++fault_events; }

void resetEvents() {
  speed_events = 0;
  fault_events = 0;
  last_average_left = 0.0f;
  last_fl = 0.0f;
  last_br = 0.0f;
}

void createSubscriber(const char *topic) {
  new WheelSpeedsSubscriber(&node, &executor, topic, onSpeed, onFault);
}

std_msgs__msg__Float32MultiArray *registeredMessage() {
  return static_cast<std_msgs__msg__Float32MultiArray *>(
      const_cast<void *>(rclc_mock::registrations[0].message));
}

void publish(const float *values, std::size_t size) {
  auto *message = registeredMessage();
  for (std::size_t index = 0; index < size; ++index) {
    message->data.data[index] = values[index];
  }
  message->data.size = size;
  rclc_mock::registrations[0].callback(message);
}

void publishFiniteMessage() {
  static const float values[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
  publish(values, 6);
}

}  // namespace

void test_constructor_registers_wheel_speed_subscription() {
  // Prepare
  resetMocks();
  resetEvents();

  // Perform
  createSubscriber("robot_w_diagnostics");

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::subscription_inits);
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::executor_adds);
  TEST_ASSERT_EQUAL_STRING("robot_w_diagnostics", rcl_mock::last_topic.c_str());
  TEST_ASSERT_EQUAL_STRING("std_msgs/msg/Float32MultiArray",
                           rcl_mock::last_type_support.c_str());
}

void test_valid_message_dispatches_typed_wheel_speeds() {
  // Prepare
  resetMocks();
  resetEvents();
  createSubscriber("robot_w_diagnostics");

  // Perform
  publishFiniteMessage();

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, speed_events);
  TEST_ASSERT_EQUAL_INT(0, fault_events);
  TEST_ASSERT_EQUAL_FLOAT(1.0f, last_average_left);
  TEST_ASSERT_EQUAL_FLOAT(3.0f, last_fl);
  TEST_ASSERT_EQUAL_FLOAT(6.0f, last_br);
}

void test_null_message_reports_fault() {
  // Prepare
  resetMocks();
  resetEvents();
  createSubscriber("robot_w_diagnostics");

  // Perform
  rclc_mock::registrations[0].callback(nullptr);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, speed_events);
  TEST_ASSERT_EQUAL_INT(1, fault_events);
}

void test_null_payload_reports_fault() {
  // Prepare
  resetMocks();
  resetEvents();
  createSubscriber("robot_w_diagnostics");
  auto *message = registeredMessage();
  message->data.data = nullptr;
  message->data.size = 6;

  // Perform
  rclc_mock::registrations[0].callback(message);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, speed_events);
  TEST_ASSERT_EQUAL_INT(1, fault_events);
}

void test_message_with_wrong_element_count_reports_fault() {
  // Prepare
  resetMocks();
  resetEvents();
  createSubscriber("robot_w_diagnostics");
  static const float values[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

  // Perform
  publish(values, 5);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, speed_events);
  TEST_ASSERT_EQUAL_INT(1, fault_events);
}

void test_non_finite_value_at_any_position_reports_fault() {
  // Prepare
  resetMocks();
  resetEvents();
  createSubscriber("robot_w_diagnostics");
  float values[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

  // Perform — every slot is rejected when it carries a NaN
  for (std::size_t index = 0; index < 6; ++index) {
    values[index] = NAN;
    publish(values, 6);
    values[index] = static_cast<float>(index) + 1.0f;
  }

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, speed_events);
  TEST_ASSERT_EQUAL_INT(6, fault_events);
}

void test_messages_dispatch_to_the_owning_subscriber_only() {
  // Prepare
  resetMocks();
  resetEvents();
  createSubscriber("robot_w_diagnostics");

  // Perform — a second valid message reaches the same callbacks
  publishFiniteMessage();
  publishFiniteMessage();

  // Asserts
  TEST_ASSERT_EQUAL_INT(2, speed_events);
  TEST_ASSERT_EQUAL_INT(0, fault_events);
}

void test_subscriber_without_callbacks_never_raises_events() {
  // Prepare
  resetMocks();
  resetEvents();
  new WheelSpeedsSubscriber(&node, &executor, "/silent", nullptr, nullptr);

  // Perform — both a valid and an invalid message arrive
  publishFiniteMessage();
  rclc_mock::registrations[0].callback(nullptr);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, speed_events);
  TEST_ASSERT_EQUAL_INT(0, fault_events);
}

void test_subscription_init_failure_is_reported_by_logger() {
  // Prepare
  resetMocks();
  resetEvents();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::subscription_init_fails = true;

  // Perform
  createSubscriber("/bad");

  // Asserts
  TEST_ASSERT_NOT_NULL(strstr(Serial.output().c_str(),
                              "[ERROR] Error to create subscriber: /bad"));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_constructor_registers_wheel_speed_subscription);
  RUN_TEST(test_valid_message_dispatches_typed_wheel_speeds);
  RUN_TEST(test_null_message_reports_fault);
  RUN_TEST(test_null_payload_reports_fault);
  RUN_TEST(test_message_with_wrong_element_count_reports_fault);
  RUN_TEST(test_non_finite_value_at_any_position_reports_fault);
  RUN_TEST(test_messages_dispatch_to_the_owning_subscriber_only);
  RUN_TEST(test_subscriber_without_callbacks_never_raises_events);
  RUN_TEST(test_subscription_init_failure_is_reported_by_logger);
  return UNITY_END();
}
