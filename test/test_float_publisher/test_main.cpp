#include <unity.h>

#include "FloatPublisher.h"
#include "NativeTestCommon.h"

static rcl_node_t node;

void test_publish_forwards_float_value() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createFloat(&node, "/f");
  FloatPublisher pub(publisher);

  // Perform
  pub.publish(1.5f);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  auto *message = static_cast<const std_msgs__msg__Float32 *>(
      rcl_mock::last_message);
  TEST_ASSERT_EQUAL_FLOAT(1.5f, message->data);
}

void test_publish_updates_value_between_calls() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createFloat(&node, "/f");
  FloatPublisher pub(publisher);

  // Perform
  pub.publish(-2.0f);
  pub.publish(7.25f);

  // Asserts
  TEST_ASSERT_EQUAL_INT(2, rcl_mock::publishes);
  auto *message = static_cast<const std_msgs__msg__Float32 *>(
      rcl_mock::last_message);
  TEST_ASSERT_EQUAL_FLOAT(7.25f, message->data);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_publish_forwards_float_value);
  RUN_TEST(test_publish_updates_value_between_calls);
  return UNITY_END();
}
