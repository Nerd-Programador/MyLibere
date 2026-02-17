#include "MyLibere.h"
#include "core/NodeIdentity.h"
#include "core/MyLibereCore.h"
#include "log/LogService.h"
#include "mqtt/MqttService.h"

MyLibereCore MyLibere::core;
MyLibere::LogMask MyLibere::_log;

// =====================================================================
// Objetos globais (Serial-like)
// Responsabilidade:
// - Dar a mesma experiência do "Serial" para MQTT e LOG.
// - Estes objetos não criam dependência nova; só encaminham para MyLibere.
// =====================================================================
MyLibereMqttShortcut Mqtt;
MyLibereLogShortcut  Log;

void MyLibere::begin() {

    Serial.println("========== BOOT ==========");

    NodeIdentity::begin();
    LogService::begin();

    Serial.print("Node ID: ");
    Serial.println(NodeIdentity::id());

    core.begin();
}

MqttService& MyLibere::mqtt() {
    return core.mqtt();
}

MyLibere::LogMask& MyLibere::log() {
    return _log;
}

void MyLibere::mqttInfo(const String& msg) {
    core.mqtt().info(msg);
}

void MyLibere::mqttData(const String& json) {
    core.mqtt().data(json);
}

void MyLibere::mqttCmd(const String& action, const String& value) {
    core.mqtt().cmd(action, value);
}

void MyLibere::logInfo(const String& msg) {
    _log.info(msg);
}

void MyLibere::logWarn(const String& msg) {
    _log.warn(msg);
}

void MyLibere::logError(const String& msg) {
    _log.error(msg);
}

/* =======================
    LogMask (Serial-like)
   ======================= */

void MyLibere::LogMask::info(const String& msg)  { LogService::info(msg); }
void MyLibere::LogMask::warn(const String& msg)  { LogService::warn(msg); }
void MyLibere::LogMask::error(const String& msg) { LogService::error(msg); }

void MyLibere::LogMask::print(const String& msg) {
    // Respeita config: pode ter log serial desativado
    #if LOG_SERIAL_ENABLED
        Serial.print(msg);
    #endif
    // Se habilitado, também joga via MQTT serial
    #if LOG_MQTT_ENABLED
        MyLibere::mqtt().print(msg);
    #endif
}

void MyLibere::LogMask::println(const String& msg) {
    print(msg + "\n");
}

void MyLibere::LogMask::println() {
    print("\n");
}


void MyLibere::loop() {
    core.loop();
    
    #if defined(ESP8266)
        yield();
    #endif

}
