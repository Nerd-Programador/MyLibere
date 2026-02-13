#pragma once
#include <Arduino.h>
#include "config/MyLibereConfig.h"

//#define LOG_BUFFER_SIZE 50

enum class LogLevel {
    INFO = 0,
    WARN,
    ERROR
};

class LogService {
public:
    static void begin(unsigned long baud = 115200);
    static void setLevel(LogLevel level);

    static void info(const String& msg);
    static void warn(const String& msg);
    static void error(const String& msg);

    static String getBuffered(uint16_t index);
    static uint16_t count();

private:
    static void log(LogLevel level, const String& msg);
    static const char* levelToString(LogLevel level);

    static LogLevel currentLevel;

    // Buffer circular
    static String buffer[LOG_BUFFER_SIZE];
    static uint16_t head;
    static uint16_t total;
};
