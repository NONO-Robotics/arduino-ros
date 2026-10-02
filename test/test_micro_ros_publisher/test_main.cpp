#include <unity.h>

#include <Arduino.h>

#include "MicroRosPublisher.h"
#include "Logger.h"
#include "NativeTestCommon.h"

static rcl_node_t node;

void test_factories_select_type_support_and_topic() {
  // Prepare
  resetMocks();

  // Perform
  MicroRosPublisher *int_pub = MicroRosPublisher::createInt(&node, "/int");
  TEST_ASSERT_EQUAL_STRING("std_msgs/msg/Int32",
                           rcl_mock::last_type_support.c_str());
  TEST_ASSERT_EQUAL_STRING("/int", rcl_mock::last_topic.c_str());

  MicroRosPublisher *float_pub = MicroRosPublisher::createFloat(&node, "/float");
  TEST_ASSERT_EQUAL_STRING("std_msgs/msg/Float32",
                           rcl_mock::last_type_support.c_str());

  MicroRosPublisher *array_pub =
      MicroRosPublisher::createFloatArray(&node, "/array");
  TEST_ASSERT_EQUAL_STRING("std_msgs/msg/Float32MultiArray",
                           rcl_mock::last_type_support.c_str());

  MicroRosPublisher *vector_pub =
      MicroRosPublisher::createVector3Stamped(&node, "/vector");
  TEST_ASSERT_EQUAL_STRING("geometry_msgs/msg/Vector3Stamped",
                           rcl_mock::last_type_support.c_str());

  MicroRosPublisher *string_pub =
      MicroRosPublisher::createString(&node, "/string");
  TEST_ASSERT_EQUAL_STRING("std_msgs/msg/String",
                           rcl_mock::last_type_support.c_str());

  // Asserts
  TEST_ASSERT_EQUAL_INT(5, rcl_mock::publisher_default_inits);
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publisher_best_effort_inits);
  delete int_pub;
  delete float_pub;
  delete array_pub;
  delete vector_pub;
  delete string_pub;
}

void test_factory_honors_best_effort_qos() {
  // Prepare
  resetMocks();

  // Perform
  MicroRosPublisher *pub =
      MicroRosPublisher::createFloat(&node, "/fast", 0, false);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publisher_default_inits);
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publisher_best_effort_inits);
  delete pub;
}

void test_publish_forwards_message_and_applies_delay() {
  // Prepare
  resetMocks();
  MicroRosPublisher pub(&node, "/delayed",
                       ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), 50);
  int payload = 42;
  arduino_mock::now_ms = 1000;

  // Perform
  pub.publish(&payload);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  TEST_ASSERT_EQUAL_PTR(&payload, rcl_mock::last_message);
  TEST_ASSERT_EQUAL_UINT32(1050, arduino_mock::now_ms);
}

void test_publish_failure_is_reported_by_logger() {
  // Prepare
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::publish_fails = true;
  MicroRosPublisher pub(&node, "/broken",
                       ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32));
  int payload = 1;

  // Perform
  pub.publish(&payload);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  TEST_ASSERT_NOT_NULL(strstr(Serial.output().c_str(),
                              "[ERROR] Error to publish message"));
}

void test_reliable_init_failure_is_reported_by_logger() {
  // Prepare
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::publisher_init_fails = true;

  // Perform
  MicroRosPublisher *pub = MicroRosPublisher::createInt(&node, "/nope");

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publisher_default_inits);
  TEST_ASSERT_NOT_NULL(strstr(Serial.output().c_str(),
                              "[ERROR] Error to create reliable publisher"));
  delete pub;
}

void test_best_effort_init_failure_is_reported_by_logger() {
  // Prepare
  resetMocks();
  Serial.clear();
  logger.begin(9600, ERROR, OUTPUT_SERIAL);
  rcl_mock::publisher_init_fails = true;

  // Perform
  MicroRosPublisher *pub =
      MicroRosPublisher::createFloat(&node, "/nope", 0, false);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publisher_best_effort_inits);
  TEST_ASSERT_NOT_NULL(strstr(
      Serial.output().c_str(),
      "[ERROR] Error to create best_effort publisher"));
  delete pub;
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_factories_select_type_support_and_topic);
  RUN_TEST(test_factory_honors_best_effort_qos);
  RUN_TEST(test_publish_forwards_message_and_applies_delay);
  RUN_TEST(test_publish_failure_is_reported_by_logger);
  RUN_TEST(test_reliable_init_failure_is_reported_by_logger);
  RUN_TEST(test_best_effort_init_failure_is_reported_by_logger);
  return UNITY_END();
}
