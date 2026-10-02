#include <unity.h>

#include "NavSatFixMsgWriter.h"
#include "NativeTestCommon.h"

void test_writer_clears_position_when_gps_has_no_fix() {
  // Prepare
  resetMocks();
  sensor_msgs__msg__NavSatFix message;
  message.latitude = 1.0;
  message.longitude = 2.0;
  message.altitude = 3.0;
  GPSData data;
  NavSatFixMsgWriter writer(&message);

  // Perform
  writer.write(&data);

  // Asserts
  TEST_ASSERT_EQUAL(sensor_msgs__msg__NavSatStatus__SERVICE_GPS,
                    message.status.service);
  TEST_ASSERT_EQUAL(sensor_msgs__msg__NavSatStatus__STATUS_NO_FIX,
                    message.status.status);
  TEST_ASSERT_EQUAL_FLOAT(0.0f, message.latitude);
  TEST_ASSERT_EQUAL_FLOAT(0.0f, message.longitude);
  TEST_ASSERT_EQUAL_FLOAT(0.0f, message.altitude);
  TEST_ASSERT_EQUAL(sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_UNKNOWN,
                    message.position_covariance_type);
}

void test_writer_maps_position_and_covariance_when_gps_has_fix() {
  // Prepare
  resetMocks();
  sensor_msgs__msg__NavSatFix message;
  GPSData data;
  tinygps_mock::setLocation(48.1173, 11.5166667, true, true);
  tinygps_mock::setHdop(0.9, true);
  tinygps_mock::setNavSatelliteAltitude(8, 545.4);
  data.encode('x');
  NavSatFixMsgWriter writer(&message);

  // Perform
  writer.write(&data);

  // Asserts
  TEST_ASSERT_EQUAL(sensor_msgs__msg__NavSatStatus__STATUS_FIX,
                    message.status.status);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 48.1173f, message.latitude);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 11.5166667f, message.longitude);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 545.4f, message.altitude);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.81f, message.position_covariance[0]);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.81f, message.position_covariance[4]);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 3.24f, message.position_covariance[8]);
  TEST_ASSERT_EQUAL(sensor_msgs__msg__NavSatFix__COVARIANCE_TYPE_DIAGONAL_KNOWN,
                    message.position_covariance_type);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_writer_clears_position_when_gps_has_no_fix);
  RUN_TEST(test_writer_maps_position_and_covariance_when_gps_has_fix);
  return UNITY_END();
}
