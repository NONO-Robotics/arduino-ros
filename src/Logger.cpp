#include "Logger.h"

#ifdef USE_ROS_LOGGER
#include "MicroRosPublisher.h"
#endif

// ── Global instance — empty constructor, no side effects ─────────────
Logger logger;

// ── begin() — call in setup() ───────────────────────────────────────────────
void Logger::begin(unsigned long baud, LogLevel level, LogOutput output)
{
    this->baud   = baud;
    this->level  = level;
    this->output = output;

    switch (output) {
        case OUTPUT_SERIAL:
            Serial.begin(baud);
            while (!Serial && millis() < 5000);
            break;

        case OUTPUT_SERIAL2:
            Serial2.begin(baud, SERIAL_8N1, 16, 17);
            while (!Serial2 && millis() < 5000);
            break;

        case OUTPUT_ROS:
            // Does not open serial. Call initRosPublisher() after the node is initialized.
            break;
    }

    initialized = true;
}

// ── Configuration ─────────────────────────────────────────────────────────────
void Logger::setLevel(LogLevel level)    { this->level  = level; }
void Logger::setOutput(LogOutput output) { this->output = output; }

// ── Internal Output ────────────────────────────────────────────────────────────
void Logger::printToOutput(const String& prefix, const String& msg)
{
    if (!initialized) return;  // silent if begin() was not called

    String full = prefix + msg;

    switch (output) {
        case OUTPUT_SERIAL:  Serial.println(full);  break;
        case OUTPUT_SERIAL2: Serial2.println(full); break;
        case OUTPUT_ROS:
#ifdef USE_ROS_LOGGER
            if (ros_log_publisher) {
                ros_log_publisher->publish(full);
            }
#endif
            break;
    }
}

// ── Main Log ─────────────────────────────────────────────────────────────
void Logger::log(LogLevel level, String msg)
{
    if (level < this->level) return;

    String prefix;
    switch (level) {
        case TRACE: prefix = "[TRACE] "; break;
        case DEBUG: prefix = "[DEBUG] "; break;
        case INFO:  prefix = "[INFO]  "; break;
        case WARN:  prefix = "[WARN]  "; break;
        case ERROR: prefix = "[ERROR] "; break;
        case FATAL: prefix = "[FATAL] "; break;
        case OFF:   return;
    }

    printToOutput(prefix, msg);
}

// ── Level Helpers ──────────────────────────────────────────────────────────
void Logger::trace(String msg) { log(TRACE, msg); }
void Logger::debug(String msg) { log(DEBUG, msg); }
void Logger::info(String msg)  { log(INFO,  msg); }
void Logger::warn(String msg)  { log(WARN,  msg); }
void Logger::error(String msg) { log(ERROR, msg); }
void Logger::fatal(String msg) { log(FATAL, msg); }

void Logger::debugPlot(String varName, float value) {
    if (isOff() || !isDebug()) return;
    printToOutput(">", varName + ":" + String(value));
}

// ── Level Checks ───────────────────────────────────────────────────────────
bool Logger::isTrace() { return level <= TRACE; }
bool Logger::isDebug() { return level <= DEBUG; }
bool Logger::isInfo()  { return level <= INFO;  }
bool Logger::isWarn()  { return level <= WARN;  }
bool Logger::isError() { return level <= ERROR; }
bool Logger::isFatal() { return level <= FATAL; }
bool Logger::isOff()   { return level == OFF;   }

#ifdef USE_ROS_LOGGER
void Logger::initRosPublisher(rcl_node_t* node)
{
    ros_log_publisher = new StringPublisher(
        MicroRosPublisher::createString(node, "/microrosout")
    );
}
#endif
