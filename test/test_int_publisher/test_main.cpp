#include <unity.h>

#include "IntPublisher.h"
#include "NativeTestCommon.h"

static rcl_node_t node;

void test_publish_forwards_integer_value() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createInt(&node, "/i");
  IntPublisher pub(publisher);

  // Perform
  pub.publish(1234);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  auto *message =
      static_cast<const std_msgs__msg__Int32 *>(rcl_mock::last_message);
  TEST_ASSERT_EQUAL_INT32(1234, message->data);
}

void test_publish_accepts_negative_and_zero_values() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher = MicroRosPublisher::createInt(&node, "/i");
  IntPublisher pub(publisher);

  // Perform
  pub.publish(-7);
  pub.publish(0);

  // Asserts
  TEST_ASSERT_EQUAL_INT(2, rcl_mock::publishes);
  auto *message =
      static_cast<const std_msgs__msg__Int32 *>(rcl_mock::last_message);
  TEST_ASSERT_EQUAL_INT32(0, message->data);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_publish_forwards_integer_value);
  RUN_TEST(test_publish_accepts_negative_and_zero_values);
  return UNITY_END();
}
