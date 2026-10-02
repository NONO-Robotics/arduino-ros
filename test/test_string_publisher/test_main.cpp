#include <unity.h>

#include "NativeTestCommon.h"
#include "StringPublisher.h"

static rcl_node_t node;

static const char *published_string() {
  auto *message =
      static_cast<const std_msgs__msg__String *>(rcl_mock::last_message);
  return message->data.data;
}

void test_publish_forwards_c_string() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createString(&node, "/s");
  StringPublisher pub(publisher);

  // Perform
  pub.publish("hello");

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  TEST_ASSERT_EQUAL_STRING("hello", published_string());
}

void test_publish_forwards_arduino_string() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createString(&node, "/s");
  StringPublisher pub(publisher);

  // Perform
  pub.publish(String("world"));

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  TEST_ASSERT_EQUAL_STRING("world", published_string());
}

void test_publish_replaces_previous_content() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createString(&node, "/s");
  StringPublisher pub(publisher);
  pub.publish("first");

  // Perform
  pub.publish("second");

  // Asserts
  TEST_ASSERT_EQUAL_INT(2, rcl_mock::publishes);
  TEST_ASSERT_EQUAL_STRING("second", published_string());
}

void test_publish_skips_when_assignment_fails() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createString(&node, "/s");
  StringPublisher pub(publisher);
  rosidl_mock::assign_fails = true;

  // Perform
  pub.publish("dropped");

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publishes);
}

void test_publish_skips_when_message_creation_fails() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createString(&node, "/s");
  std_msgs_string_mock::create_fails = true;
  StringPublisher pub(publisher);

  // Perform
  pub.publish("ghost");

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publishes);
}

void test_publish_skips_without_publisher() {
  // Prepare
  resetMocks();
  StringPublisher pub(nullptr);

  // Perform
  pub.publish("no publisher");

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publishes);
}

void test_destructor_destroys_message() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createString(&node, "/s");

  // Perform
  {
    StringPublisher pub(publisher);
  }

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, std_msgs_string_mock::destroys);
}

void test_destructor_handles_failed_message_creation() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createString(&node, "/s");
  std_msgs_string_mock::create_fails = true;

  // Perform
  {
    StringPublisher pub(publisher);
  }

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, std_msgs_string_mock::destroys);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_publish_forwards_c_string);
  RUN_TEST(test_publish_forwards_arduino_string);
  RUN_TEST(test_publish_replaces_previous_content);
  RUN_TEST(test_publish_skips_when_assignment_fails);
  RUN_TEST(test_publish_skips_when_message_creation_fails);
  RUN_TEST(test_publish_skips_without_publisher);
  RUN_TEST(test_destructor_destroys_message);
  RUN_TEST(test_destructor_handles_failed_message_creation);
  return UNITY_END();
}
