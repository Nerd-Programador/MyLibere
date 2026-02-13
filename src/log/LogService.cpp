#include "LogService.h"
#include "utils/TimeUtils.h"
#include "core/NodeIdentity.h"
#include "bridge/SerialBridgeService.h"

LogLevel LogService::currentLevel = LogLevel::INFO;

String LogService::buffer[LOG_BUFFER_SIZE];
uint16_t LogService::head = 0;
uint16_t LogService::total = 0;

void LogService::begin(unsigned long baud) {
    Serial.begin(baud);
}

void LogService::setLevel(LogLevel level) {
    currentLevel = level;
}

void LogService::info(const String& msg) {
    log(LogLevel::INFO, msg);

    #if SERIAL_BRIDGE_ENABLED
        SerialBridgeService::log(msg, "info");
    #endif
}

void LogService::warn(const String& msg) {
    log(LogLevel::WARN, msg);

    #if SERIAL_BRIDGE_ENABLED
        SerialBridgeService::log(msg, "warn");
    #endif
}

void LogService::error(const String& msg) {
    log(LogLevel::ERROR, msg);

    #if SERIAL_BRIDGE_ENABLED
        SerialBridgeService::log(msg, "error");
    #endif
}

void LogService::log(LogLevel level, const String& msg) {

    if (level < currentLevel) return;

    // Monta log formatado
    String formatted =
        "[" + String(TimeUtils::now()) + "] " +
        "[" + NodeIdentity::id() + "] " +
        levelToString(level) + ": " +
        msg;

    // Serial
    Serial.println(formatted);

    // Armazena no buffer circular
    buffer[head] = formatted;
    head = (head + 1) % LOG_BUFFER_SIZE;

    if (total < LOG_BUFFER_SIZE)
        total++;
}

String LogService::getBuffered(uint16_t index) {

    if (index >= total) return "";

    uint16_t pos = (head + LOG_BUFFER_SIZE - total + index) % LOG_BUFFER_SIZE;
    return buffer[pos];
}

uint16_t LogService::count() {
    return total;
}

const char* LogService::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default:              return "UNK";
    }
}
