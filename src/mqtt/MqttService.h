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

    /* =======================
        API "máscara" (Serial-like)
        Objetivo: chamadas curtas e difíceis de errar
       ======================= */

    // Publica em: servermaster/<NodeID>/<ENV>/<subtopic>
    bool pub(const String& subtopic, const String& payload, bool retain = false);

    // Atalhos por canal
    bool status(const String& payload, bool retain = true);
    bool data(const String& payload, bool retain = false);
    bool tele(const String& payload, bool retain = false);
    bool event(const String& type, const String& payloadJson = "{}");

    // Logs via MQTT (canal serial)
    bool info(const String& msg);
    bool warn(const String& msg);
    bool error(const String& msg);

    // Estilo Serial
    bool print(const String& msg);
    bool println(const String& msg);
    bool println();

    // Comando padrão (JSON)
    bool cmd(const String& action, const String& value);

    /* -----------------------
       Sobrecargas de cmd
       -----------------------
       Responsabilidade:
       - Permitir chamadas curtas: Mqtt.cmd("luz", true)
       - Evitar conversões manuais para String no firmware
       - Padronizar bool como "ON" / "OFF"
    */
    bool cmd(const String& action, bool value);
    bool cmd(const String& action, int value);
    bool cmd(const String& action, float value, uint8_t decimals = 2);

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

    // Helpers de tópico (padrão único)
    static String baseTopic_();
    static String topicStatus_();
    static String topicData_();
    static String topicTelemetry_();
    static String topicEvent_();
    static String topicCmdWildcard_();
    static String topicSerial_();

};
