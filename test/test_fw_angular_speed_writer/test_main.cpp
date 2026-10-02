#include <unity.h>

#include "FWAngularSpeedWriter.h"
#include "NativeTestCommon.h"

void test_mecanum_strategy_computes_all_wheels() {
  // Prepare
  resetMocks();
  MecanumStrategy strategy(0.4f, 0.2f, 0.1f);
  FourWheelAngularSpeed speeds;

  // Perform
  strategy.compute(1.0f, 0.5f, 2.0f, speeds);

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, -7.0f, speeds.getFlWInRad());
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 27.0f, speeds.getFrWInRad());
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3.0f, speeds.getBlWInRad());
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 17.0f, speeds.getBrWInRad());
}

void test_skid_steer_strategy_computes_all_wheels() {
  // Prepare
  resetMocks();
  SkidSteerStrategy strategy(0.6f, 0.1f, 0.2f);
  FourWheelAngularSpeed speeds;

  // Perform
  strategy.compute(1.0f, 99.0f, 2.0f, speeds);

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 16.0f, speeds.getFlWInRad());
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 4.0f, speeds.getFrWInRad());
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 8.0f, speeds.getBlWInRad());
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.0f, speeds.getBrWInRad());
}

void test_writer_delegates_twist_to_strategy() {
  // Prepare
  resetMocks();
  MecanumStrategy strategy(0.5f, 0.5f, 0.5f);
  FourWheelAngularSpeed speeds;
  FWAngularSpeedWriter writer(&strategy, &speeds);
  geometry_msgs__msg__Twist twist;
  twist.linear.x = 1.0;
  twist.linear.y = 0.5;
  twist.angular.z = 0.25;

  // Perform
  writer.write(&twist);

  // Asserts
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.5f, speeds.getFlWInRad());
  TEST_ASSERT_EQUAL_PTR(&speeds, &writer.getAngularSpeed());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_mecanum_strategy_computes_all_wheels);
  RUN_TEST(test_skid_steer_strategy_computes_all_wheels);
  RUN_TEST(test_writer_delegates_twist_to_strategy);
  return UNITY_END();
}
