#include <unity.h>

#include <Arduino.h>
#define private public
#include "Logger.h"
#undef private
#include "NativeTestCommon.h"

void test_logger_is_silent_before_begin() {
  // Prepare
  resetMocks();
  Serial.clear();
  Logger logger;

  // Perform
  logger.info("quiet");

  // Asserts
  TEST_ASSERT_EQUAL_STRING("", Serial.output().c_str());
}

void test_logger_writes_all_levels_to_serial() {
  // Prepare
  resetMocks();
  Serial.clear();
  Logger logger;
  logger.begin(115200, TRACE, OUTPUT_SERIAL);

  // Perform
  logger.trace("one");
  logger.debug("two");
  logger.info("three");
  logger.warn("four");
  logger.error("five");
  logger.fatal("six");

  // Asserts
  TEST_ASSERT_EQUAL_UINT32(115200, Serial.baud);
  TEST_ASSERT_EQUAL_STRING("[TRACE] one\n[DEBUG] two\n[INFO]  three\n[WARN]  four\n[ERROR] five\n[FATAL] six\n",
                           Serial.output().c_str());
}

void test_logger_filters_levels_and_handles_off() {
  // Prepare
  resetMocks();
  Serial.clear();
  Logger logger;
  logger.begin(9600, WARN, OUTPUT_SERIAL);

  // Perform
  logger.info("hidden");
  logger.warn("shown");
  logger.setLevel(OFF);
  logger.fatal("hidden too");

  // Asserts
  TEST_ASSERT_TRUE(logger.isOff());
  TEST_ASSERT_FALSE(logger.isFatal());
  TEST_ASSERT_EQUAL_STRING("[WARN]  shown\n", Serial.output().c_str());
}

void test_logger_writes_serial2_and_ros_stays_host_silent() {
  // Prepare
  resetMocks();
  Serial2.clear();
  Logger logger;
  logger.begin(57600, DEBUG, OUTPUT_SERIAL2);

  // Perform
  logger.debugPlot("speed", 3.5f);
  logger.setOutput(OUTPUT_ROS);
  logger.info("host path");

  // Asserts
  TEST_ASSERT_EQUAL_UINT32(57600, Serial2.baud);
  TEST_ASSERT_EQUAL_STRING(">speed:3.500000\n", Serial2.output().c_str());
  TEST_ASSERT_TRUE(logger.isDebug());
  TEST_ASSERT_TRUE(logger.isInfo());
  TEST_ASSERT_TRUE(logger.isWarn());
  TEST_ASSERT_TRUE(logger.isError());
}

void test_logger_handles_unavailable_serial_ports() {
  // Prepare
  resetMocks();
  arduino_mock::now_ms = 5000;
  Serial.connected = false;
  Serial2.connected = false;
  Logger serial_logger;
  Logger serial2_logger;

  // Perform
  serial_logger.begin(9600, INFO, OUTPUT_SERIAL);
  serial2_logger.begin(19200, INFO, OUTPUT_SERIAL2);

  // Asserts
  TEST_ASSERT_EQUAL_UINT32(9600, Serial.baud);
  TEST_ASSERT_EQUAL_UINT32(19200, Serial2.baud);
}

void test_logger_handles_all_local_control_paths() {
  // Prepare
  resetMocks();
  Serial.clear();
  Logger logger;
  logger.begin(9600, INFO, OUTPUT_ROS);

  // Perform
  logger.debugPlot("hidden", 1.0f);
  logger.setLevel(OFF);
  logger.debugPlot("off", 2.0f);
  logger.setLevel(TRACE);
  logger.log(OFF, "ignored");
  logger.setOutput(static_cast<LogOutput>(99));
  logger.info("discarded");
  logger.log(static_cast<LogLevel>(99), "default");

  // Asserts
  TEST_ASSERT_EQUAL_STRING("", Serial.output().c_str());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_logger_is_silent_before_begin);
  RUN_TEST(test_logger_writes_all_levels_to_serial);
  RUN_TEST(test_logger_filters_levels_and_handles_off);
  RUN_TEST(test_logger_writes_serial2_and_ros_stays_host_silent);
  RUN_TEST(test_logger_handles_unavailable_serial_ports);
  RUN_TEST(test_logger_handles_all_local_control_paths);
  return UNITY_END();
}
