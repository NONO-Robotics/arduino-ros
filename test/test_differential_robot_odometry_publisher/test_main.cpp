#include <unity.h>

#include "DifferentialRobotOdometryPublisher.h"
#include "NativeTestCommon.h"

static rcl_node_t node;

static DifferentialRobotOdometry makeOdometry() {
  DifferentialRobotOdometry odometry;
  FourWheelAngularSpeed speeds;
  speeds.updateFrom(1.0f, 1.0f, 1.0f, 1.0f);
  speeds.setBrWInRad(2.0f);
  odometry.updateFrom(speeds);
  return odometry;
}

void test_constructor_uses_default_topic() {
  // Prepare
  resetMocks();

  // Perform
  DifferentialRobotOdometryPublisher pub(&node);

  // Asserts
  TEST_ASSERT_EQUAL_STRING("odometry", rcl_mock::last_topic.c_str());
}

void test_constructor_honors_custom_topic_and_frame() {
  // Prepare
  resetMocks();

  // Perform
  DifferentialRobotOdometryPublisher pub(&node, "wheel_odom", "odom");

  // Asserts
  TEST_ASSERT_EQUAL_STRING("wheel_odom", rcl_mock::last_topic.c_str());
}

void test_publish_maps_odometry_to_vector_message() {
  // Prepare
  resetMocks();
  DifferentialRobotOdometry odometry = makeOdometry();
  DifferentialRobotOdometryPublisher pub(&node, "wheel_odom", "odom");

  // Perform
  pub.publish(odometry);

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, rcl_mock::publishes);
  auto *message = static_cast<const geometry_msgs__msg__Vector3Stamped *>(
      rcl_mock::last_message);
  TEST_ASSERT_EQUAL_STRING("odom", message->header.frame_id.data);
  TEST_ASSERT_EQUAL_FLOAT(odometry.getLeftWInRad(), message->vector.x);
  TEST_ASSERT_EQUAL_FLOAT(odometry.getRightWInRad(), message->vector.y);
}

void test_destructor_releases_owned_publisher() {
  // Prepare
  resetMocks();

  // Perform
  {
    DifferentialRobotOdometryPublisher pub(&node);
  }

  // Asserts
  TEST_ASSERT_EQUAL_INT(1, geometry_msgs_mock::destroys);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_constructor_uses_default_topic);
  RUN_TEST(test_constructor_honors_custom_topic_and_frame);
  RUN_TEST(test_publish_maps_odometry_to_vector_message);
  RUN_TEST(test_destructor_releases_owned_publisher);
  return UNITY_END();
}
