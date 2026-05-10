#pragma once
#include <Arduino.h>
#include "StringPublisher.h"
#include "MicroRosPublisher.h"
#include <rcl/rcl.h>

enum LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF };

enum LogOutput {
    OUTPUT_SERIAL,
    OUTPUT_SERIAL2,
    OUTPUT_ROS
};

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

    void setLevel(LogLevel level);
    void setOutput(LogOutput output);

    bool isDebug();
    bool isTrace();
    bool isInfo();
    bool isWarn();
    bool isError();
    bool isFatal();
    bool isOff();

    void debugPlot(String varName, float value);

    void trace(String msg);
    void debug(String msg);
    void info(String msg);
    void warn(String msg);
    void error(String msg);
    void fatal(String msg);
    void initRosPublisher(rcl_node_t* node);

private:
    LogLevel  level  = INFO;
    LogOutput output = OUTPUT_SERIAL;
    unsigned long baud = 115200;
    bool initialized = false;
    StringPublisher* ros_log_publisher = nullptr;

    void log(LogLevel level, String msg);
    void printToOutput(const String& prefix, const String& msg);
};

extern Logger logger;
