#include <unity.h>

#include "IMUMsgWriter.h"
#include "NativeTestCommon.h"

void test_writer_maps_every_imu_measurement() {
  // Prepare
  resetMocks();
  sensor_msgs__msg__Imu message;
  IMUData data;
  data.setOrientation(1.0, 2.0, 3.0, 4.0);
  data.setAngularVelocity(5.0, 6.0, 7.0);
  data.setLinearAcceleration(8.0, 9.0, 10.0);
  IMUMsgWriter writer(&message);

  // Perform
  writer.write(&data);

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, message.orientation.w);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.0f, message.orientation.x);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3.0f, message.orientation.y);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 4.0f, message.orientation.z);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 5.0f, message.angular_velocity.x);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 6.0f, message.angular_velocity.y);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 7.0f, message.angular_velocity.z);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 8.0f, message.linear_acceleration.x);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 9.0f, message.linear_acceleration.y);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 10.0f, message.linear_acceleration.z);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_writer_maps_every_imu_measurement);
  return UNITY_END();
}
