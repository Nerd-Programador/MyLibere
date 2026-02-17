#include "MqttService.h"

#if defined(ESP32)
    #include <WiFi.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include "config/MqttDefaults.h"
#include "config/EnvironmentConfig.h"
#include "network/WiFiService.h"
#include "utils/TimeUtils.h"
#include "core/NodeIdentity.h"
#include "log/LogService.h"

MqttService::MqttService()
: _mqtt(_wifiClient),
_lastReconnect(0)
{}

void MqttService::begin() {

    Serial.println("Inicializando MQTT...");

    _mqtt.setServer(MQTT_BROKER, MQTT_PORT);
    _mqtt.setCallback(MqttService::onMessage);
}


void MqttService::loop() {
    if (!WiFiService::isConnected()) return;

    if (_mqtt.connected()) {
        _mqtt.loop();
        return;
    }

    if (TimeUtils::elapsed(_lastReconnect, MQTT_RECONNECT_INTERVAL)) {
        TimeUtils::reset(_lastReconnect);
        reconnect();
    }
}

bool MqttService::reconnect() {

    const String nodeId = NodeIdentity::id();

    LogService::info("Tentando conectar MQTT...");

    String willTopic = topicStatus_();

    bool connected = _mqtt.connect(
        nodeId.c_str(),
        MQTT_USER,
        MQTT_PASS,
        willTopic.c_str(),
        1,
        true,
        "offline"
    );

    if (connected) {

        LogService::info("MQTT conectado");

        // Publica ONLINE
        _mqtt.publish(
            willTopic.c_str(),
            "online",
            true
        );

        // Assina comandos do ambiente (padrão único)
        _mqtt.subscribe(topicCmdWildcard_().c_str());

        // Publica HELLO inicial (igual seu teste MQTT-FB)
        publishHello();

    } else {

        LogService::warn("Falha ao conectar MQTT");
    }

    return connected;
}

bool MqttService::isConnected() {
    return _mqtt.connected();
}

void MqttService::onMessage(char* topic,
                            byte* payload,
                            unsigned int length) {

    String topicStr = String(topic);

    // Constrói payload em String
    String msg;
    msg.reserve(length);

    for (unsigned int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }

    LogService::info("MQTT RX: " + topicStr + " -> " + msg);

    // Padrão de comandos: servermaster/<NodeID>/<ENV>/cmd/<canal>
    const String cmdPrefix = baseTopic_() + "/cmd/";
    if (!topicStr.startsWith(cmdPrefix)) return;

    // Parse JSON (para callback do usuário)
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, msg);
    if (error) {
        LogService::warn("JSON inválido");
        return;
    }

    if (_commandCallback != nullptr) {
        String action = doc["action"] | "";
        String value  = doc["value"]  | "";
        _commandCallback(action, value, doc);
    }

    const String channel = topicStr.substring(cmdPrefix.length());
    if (channel == "do") {
        handleCommand(msg);
    } else if (channel == "config") {
        handleConfig(msg);
    } else if (channel == "ota") {
        handleOta(msg);
    }
}

void MqttService::publishHello() {

    const String nodeId = NodeIdentity::id();

    String topic = topicStatus_();

    String payload = "{";
    payload += "\"chip\":\"" + nodeId + "\",";
    payload += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
    payload += "}";

    _mqtt.publish(topic.c_str(), payload.c_str(), true);
}

void MqttService::publishData(const String& payload, bool retain) {

    if (!_mqtt.connected()) return;

    _mqtt.publish(topicData_().c_str(), payload.c_str(), retain);
}

void MqttService::publishSerial(const String& message) {

    if (!_mqtt.connected()) return;

    _mqtt.publish(topicSerial_().c_str(), message.c_str(), false);
}

/* =======================
    API "máscara" (Serial-like)
   ======================= */

bool MqttService::pub(const String& subtopic, const String& payload, bool retain) {
    if (!_mqtt.connected()) return false;
    const String t = baseTopic_() + "/" + subtopic;
    return _mqtt.publish(t.c_str(), payload.c_str(), retain);
}

bool MqttService::status(const String& payload, bool retain) {
    if (!_mqtt.connected()) return false;
    return _mqtt.publish(topicStatus_().c_str(), payload.c_str(), retain);
}

bool MqttService::data(const String& payload, bool retain) {
    publishData(payload, retain);
    return _mqtt.connected();
}

bool MqttService::tele(const String& payload, bool retain) {
    if (!_mqtt.connected()) return false;
    return _mqtt.publish(topicTelemetry_().c_str(), payload.c_str(), retain);
}

bool MqttService::event(const String& type, const String& payloadJson) {
    if (!_mqtt.connected()) return false;
    // JSON pequeno e padrão
    String msg = String("{\"type\":\"") + type + "\",\"data\":" + payloadJson + "}";
    return _mqtt.publish(topicEvent_().c_str(), msg.c_str(), false);
}

bool MqttService::info(const String& msg)  { return println(String("[INFO] ")  + msg); }
bool MqttService::warn(const String& msg)  { return println(String("[WARN] ")  + msg); }
bool MqttService::error(const String& msg) { return println(String("[ERROR] ") + msg); }

bool MqttService::print(const String& msg) {
    if (!_mqtt.connected()) return false;
    return _mqtt.publish(topicSerial_().c_str(), msg.c_str(), false);
}

bool MqttService::println(const String& msg) {
    return print(msg + "\n");
}

bool MqttService::println() {
    return print("\n");
}

bool MqttService::cmd(const String& action, const String& value) {
    if (!_mqtt.connected()) return false;
    String payload = "{";
    payload += "\"action\":\"" + action + "\",";
    payload += "\"value\":\"" + value + "\"";
    payload += "}";
    const String t = baseTopic_() + "/cmd/do";
    return _mqtt.publish(t.c_str(), payload.c_str(), false);
}

bool MqttService::cmd(const String& action, bool value) {
    // Converte bool para algo humano no broker: ON/OFF
    return cmd(action, value ? "ON" : "OFF");
}

bool MqttService::cmd(const String& action, int value) {
    return cmd(action, String(value));
}

bool MqttService::cmd(const String& action, float value, uint8_t decimals) {
    // Converte float de forma compatível com ESP32 e ESP8266
    // dtostrf evita ambiguidade do construtor String(float, decimals) no core do ESP32
    char buf[24];
    dtostrf(value, 0, decimals, buf);  // width=0, precision=decimals
    String s(buf);
    s.trim(); // remove espaços que o dtostrf pode colocar no começo
    return cmd(action, s);
}

/* =======================
    Helpers de tópico
   ======================= */

String MqttService::baseTopic_() {
    // servermaster/<NodeID>/<ENV>
    return String(MQTT_ROOT_TOPIC) + "/" + NodeIdentity::id() + "/" + String(DEVICE_ENVIRONMENT);
}

String MqttService::topicStatus_()      { return baseTopic_() + "/status"; }
String MqttService::topicData_()        { return baseTopic_() + "/data"; }
String MqttService::topicTelemetry_()   { return baseTopic_() + "/telemetry"; }
String MqttService::topicEvent_()       { return baseTopic_() + "/event"; }
String MqttService::topicCmdWildcard_() { return baseTopic_() + "/cmd/#"; }
String MqttService::topicSerial_()      { return baseTopic_() + "/serial"; }

CommandCallback MqttService::_commandCallback = nullptr;

void MqttService::onCommand(CommandCallback callback) {
    _commandCallback = callback;
}


void MqttService::handleCommand(const String& payload) {
    LogService::info("Handle CMD: " + payload);
}

void MqttService::handleConfig(const String& payload) {
    LogService::info("Handle CONFIG: " + payload);
}

void MqttService::handleOta(const String& payload) {
    LogService::info("Handle OTA: " + payload);
}
