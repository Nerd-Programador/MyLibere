#include "MyLibere.h"
#include "core/NodeIdentity.h"
#include "core/MyLibereCore.h"
#include "log/LogService.h"
#include "mqtt/MqttService.h"

MyLibereCore MyLibere::core;

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

void MyLibere::mqttInfo(const String& msg) {
    core.mqtt().publishSerial(msg);
}

void MyLibere::mqttData(const String& json) {
    core.mqtt().publishData(json);
}

void MyLibere::mqttCmd(const String& action, const String& value) {

    String payload = "{";
    payload += "\"action\":\"" + action + "\",";
    payload += "\"value\":\"" + value + "\"";
    payload += "}";

    core.mqtt().publishData(payload);
}

void MyLibere::logInfo(const String& msg) {
    LogService::info(msg);
}

void MyLibere::logWarn(const String& msg) {
    LogService::warn(msg);
}

void MyLibere::logError(const String& msg) {
    LogService::error(msg);
}


void MyLibere::loop() {
    core.loop();
    
    #if defined(ESP8266)
        yield();
    #endif

}
