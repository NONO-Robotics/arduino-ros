#pragma once
#include <Arduino.h>
#include <rcl/rcl.h>
#ifdef USE_ROS_LOGGER
#include "StringPublisher.h"
#include "MicroRosPublisher.h"
#else
class StringPublisher;
#endif

enum LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF };

enum LogOutput {
    OUTPUT_SERIAL,
    OUTPUT_SERIAL2,
    OUTPUT_ROS
};

/** @brief Routes application log messages to serial output or ROS. */
class Logger {
public:
    /**
     * @brief Empty constructor — does nothing.
     *        Call begin() in setup() to initialize.
     */
    Logger() = default;

    /**
     * @brief Initializes the logger with the chosen destination.
     *        Call in setup() before using any log.
     */
    void begin(unsigned long baud, LogLevel level = INFO, LogOutput output = OUTPUT_SERIAL);

    /** @brief Set minimum severity emitted by this logger. @param level Minimum severity. */
    void setLevel(LogLevel level);
    /** @brief Set destination for subsequent log messages. @param output Output destination. */
    void setOutput(LogOutput output);

    /** @brief Check whether debug messages are enabled. @return True when enabled. */
    bool isDebug();
    /** @brief Check whether trace messages are enabled. @return True when enabled. */
    bool isTrace();
    /** @brief Check whether info messages are enabled. @return True when enabled. */
    bool isInfo();
    /** @brief Check whether warning messages are enabled. @return True when enabled. */
    bool isWarn();
    /** @brief Check whether error messages are enabled. @return True when enabled. */
    bool isError();
    /** @brief Check whether fatal messages are enabled. @return True when enabled. */
    bool isFatal();
    /** @brief Check whether logging is disabled. @return True when disabled. */
    bool isOff();

    /** @brief Emit a named numeric value for serial plotting. @param varName Series name. @param value Sample value. */
    void debugPlot(String varName, float value);

    /** @brief Emit a trace message. @param msg Message text. */
    void trace(String msg);
    /** @brief Emit a debug message. @param msg Message text. */
    void debug(String msg);
    /** @brief Emit an informational message. @param msg Message text. */
    void info(String msg);
    /** @brief Emit a warning message. @param msg Message text. */
    void warn(String msg);
    /** @brief Emit an error message. @param msg Message text. */
    void error(String msg);
    /** @brief Emit a fatal message. @param msg Message text. */
    void fatal(String msg);
    /** @brief Initialize ROS log publishing. @param node Initialized ROS node used by publisher. */
    void initRosPublisher(rcl_node_t* node);

private:
    LogLevel  level  = INFO;
    LogOutput output = OUTPUT_SERIAL;
    unsigned long baud = 921600;
    bool initialized = false;
    StringPublisher* ros_log_publisher = nullptr;

    void log(LogLevel level, String msg);
    void printToOutput(const String& prefix, const String& msg);
};

extern Logger logger;
