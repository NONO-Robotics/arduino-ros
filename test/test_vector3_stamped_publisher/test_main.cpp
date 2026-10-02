#include <unity.h>

#include "NativeTestCommon.h"
#include "Vector3StampedPublisher.h"

static rcl_node_t node;

void test_publish_sets_vector_and_default_frame() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createVector3Stamped(&node, "/v");
  Vector3StampedPublisher pub(publisher);

  // Perform
  pub.publish(1.0f, -2.0f, 0.5f);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  TEST_ASSERT_EQUAL_INT(1, time_mock::stamps);
  auto *message = static_cast<const geometry_msgs__msg__Vector3Stamped *>(
      rcl_mock::last_message);
  TEST_ASSERT_EQUAL_STRING("base_link", message->header.frame_id.data);
  TEST_ASSERT_EQUAL_FLOAT(1.0f, message->vector.x);
  TEST_ASSERT_EQUAL_FLOAT(-2.0f, message->vector.y);
  TEST_ASSERT_EQUAL_FLOAT(0.5f, message->vector.z);
}

void test_constructor_honors_explicit_frame_id() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createVector3Stamped(&node, "/v");
  Vector3StampedPublisher pub(publisher, "odom");

  // Perform
  pub.publish();

  // Asserts
  auto *message = static_cast<const geometry_msgs__msg__Vector3Stamped *>(
      rcl_mock::last_message);
  TEST_ASSERT_EQUAL_STRING("odom", message->header.frame_id.data);
}

void test_publish_returns_early_without_message() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createVector3Stamped(&node, "/v");
  geometry_msgs_mock::create_fails = true;
  Vector3StampedPublisher pub(publisher);

  // Perform
  pub.publish(9.0f, 9.0f, 9.0f);

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, rcl_mock::publishes);
  TEST_ASSERT_EQUAL_INT(0, time_mock::stamps);
}

void test_destructor_destroys_message_and_publisher() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createVector3Stamped(&node, "/v");

  // Perform
  {
    Vector3StampedPublisher pub(publisher);
  }

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, geometry_msgs_mock::destroys);
}

void test_destructor_handles_failed_message_creation() {
  // Prepare
  resetMocks();
  MicroRosPublisher *publisher =
      MicroRosPublisher::createVector3Stamped(&node, "/v");
  geometry_msgs_mock::create_fails = true;

  // Perform
  {
    Vector3StampedPublisher pub(publisher);
  }

  // Asserts
  TEST_ASSERT_EQUAL_INT(0, geometry_msgs_mock::destroys);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_publish_sets_vector_and_default_frame);
  RUN_TEST(test_constructor_honors_explicit_frame_id);
  RUN_TEST(test_publish_returns_early_without_message);
  RUN_TEST(test_destructor_destroys_message_and_publisher);
  RUN_TEST(test_destructor_handles_failed_message_creation);
  return UNITY_END();
}
