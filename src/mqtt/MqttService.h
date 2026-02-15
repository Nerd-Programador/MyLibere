#pragma once
#include <Arduino.h>

#if defined(ESP32)
    #include <WiFi.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include <ArduinoJson.h>

typedef void (*CommandCallback)(
    const String& action,
    const String& value,
    JsonDocument& doc
);

class MqttService {
public:
    MqttService();

    void begin();
    void loop();

    bool isConnected();
    /*void publish(const String& topic,
                const String& payload,
                bool retain = false);*/

    void publishHello();
    void publishData(const String& payload, bool retain = false);
    void publishSerial(const String& message);

    void onCommand(CommandCallback callback);

private:
    WiFiClient _wifiClient;   // agora o tipo já é conhecido
    PubSubClient _mqtt;
    unsigned long _lastReconnect;

    bool reconnect();
    static void onMessage(char* topic,
                        byte* payload,
                        unsigned int length);

    static CommandCallback _commandCallback;

    static void handleCommand(const String& payload);
    static void handleConfig(const String& payload);
    static void handleOta(const String& payload);

};
