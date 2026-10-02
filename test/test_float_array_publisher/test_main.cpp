#include <unity.h>

#include <limits>

#include "FloatArrayPublisher.h"
#include "NativeTestCommon.h"

static rcl_node_t node;

void test_publish_copies_array_contents() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createFloatArray(&node, "/array");
  FloatArrayPublisher pub(publisher, 3);
  float values[3] = {1.0f, -2.5f, 3.75f};

  // Perform
  pub.publish(values);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  auto *message = static_cast<const std_msgs__msg__Float32MultiArray *>(
      rcl_mock::last_message);
  TEST_ASSERT_EQUAL_UINT32(3, message->data.size);
  TEST_ASSERT_EQUAL_FLOAT(1.0f, message->data.data[0]);
  TEST_ASSERT_EQUAL_FLOAT(-2.5f, message->data.data[1]);
  TEST_ASSERT_EQUAL_FLOAT(3.75f, message->data.data[2]);
}

void test_destructor_releases_owned_message() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createFloatArray(&node, "/array");

  // Perform
  {
    FloatArrayPublisher pub(publisher, 2);
  }

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publishes);
}

void test_destructor_handles_failed_message_allocation() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createFloatArray(&node, "/array");

  // Perform
  { FloatArrayPublisher pub(publisher, std::numeric_limits<size_t>::max()); }

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publishes);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_publish_copies_array_contents);
  RUN_TEST(test_destructor_releases_owned_message);
  RUN_TEST(test_destructor_handles_failed_message_allocation);
  return UNITY_END();
}
