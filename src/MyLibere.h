#pragma once

#include "core/MyLibereCore.h"

class MyLibere {
public:
    static void begin();
    static void loop();

    static MqttService& mqtt();

    // Atalhos MQTT
    static void mqttInfo(const String& msg);
    static void mqttCmd(const String& action, const String& value);
    static void mqttData(const String& json);

    // Atalhos Log
    static void logInfo(const String& msg);
    static void logWarn(const String& msg);
    static void logError(const String& msg);

private:
    static MyLibereCore core;
};
