#include <unity.h>

#include "Int32ArrayPublisher.h"
#include "NativeTestCommon.h"

static rcl_node_t node;

void test_publish_copies_array_contents() {
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createInt32Array(&node, "/array");
  Int32ArrayPublisher pub(publisher, 4);
  int32_t values[4] = {3, -17, 0, 250};

  pub.publish(values);

  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  auto *message = static_cast<const std_msgs__msg__Int32MultiArray *>(
      rcl_mock::last_message);
  TEST_ASSERT_EQUAL_UINT32(4, message->data.size);
  TEST_ASSERT_EQUAL_INT32(3, message->data.data[0]);
  TEST_ASSERT_EQUAL_INT32(-17, message->data.data[1]);
  TEST_ASSERT_EQUAL_INT32(0, message->data.data[2]);
  TEST_ASSERT_EQUAL_INT32(250, message->data.data[3]);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_publish_copies_array_contents);
  return UNITY_END();
}
