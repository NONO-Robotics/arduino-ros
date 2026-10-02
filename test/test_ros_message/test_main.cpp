#include <cstdlib>
#include <limits>

#include <unity.h>

#include "NativeTestCommon.h"
#include "RosMessage.h"

void test_scalar_messages_are_zero_initialized() {
  // Prepare
  resetMocks();

  // Perform
  std_msgs__msg__Float32* float_message = createFloatMessage();
  std_msgs__msg__Int32* int_message = createIntMessage();

  // Asserts
  TEST_ASSERT_EQUAL_FLOAT(0.0f, float_message->data);
  TEST_ASSERT_EQUAL_INT32(0, int_message->data);
  delete float_message;
  delete int_message;
}

void test_float_array_message_allocates_requested_length() {
  // Prepare
  resetMocks();

  // Perform
  std_msgs__msg__Float32MultiArray* message = createFloatArrayMessage(3);

  // Asserts
  TEST_ASSERT_NOT_NULL(message);
  TEST_ASSERT_NOT_NULL(message->data.data);
  TEST_ASSERT_EQUAL_UINT32(3, message->data.size);
  TEST_ASSERT_EQUAL_UINT32(3, message->data.capacity);
  std::free(message->data.data);
  delete message;
}

void test_float_array_message_returns_null_when_allocation_fails() {
  // Prepare
  resetMocks();

  // Perform
  std_msgs__msg__Float32MultiArray* message =
      createFloatArrayMessage(std::numeric_limits<size_t>::max());

  // Asserts
  TEST_ASSERT_NULL(message);
}

void test_vector_message_assigns_frame_id() {
  // Prepare
  resetMocks();

  // Perform
  geometry_msgs__msg__Vector3Stamped* message =
      createVector3StampedMessage("base_link");

  // Asserts
  TEST_ASSERT_NOT_NULL(message);
  TEST_ASSERT_EQUAL_STRING("base_link", message->header.frame_id.data);
  geometry_msgs__msg__Vector3Stamped__destroy(message);
}

void test_vector_message_returns_null_when_create_fails() {
  // Prepare
  resetMocks();
  geometry_msgs_mock::create_fails = true;

  // Perform
  geometry_msgs__msg__Vector3Stamped* message =
      createVector3StampedMessage("base_link");

  // Asserts
  TEST_ASSERT_NULL(message);
}

void test_vector_message_destroys_message_when_assign_fails() {
  // Prepare
  resetMocks();
  rosidl_mock::assign_fails = true;

  // Perform
  geometry_msgs__msg__Vector3Stamped* message =
      createVector3StampedMessage("base_link");

  // Asserts
  TEST_ASSERT_NULL(message);
  TEST_ASSERT_EQUAL_INT(1, geometry_msgs_mock::destroys);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_scalar_messages_are_zero_initialized);
  RUN_TEST(test_float_array_message_allocates_requested_length);
  RUN_TEST(test_float_array_message_returns_null_when_allocation_fails);
  RUN_TEST(test_vector_message_assigns_frame_id);
  RUN_TEST(test_vector_message_returns_null_when_create_fails);
  RUN_TEST(test_vector_message_destroys_message_when_assign_fails);
  return UNITY_END();
}
