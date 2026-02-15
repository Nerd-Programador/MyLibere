#include "MqttService.h"

#if defined(ESP32)
    #include <WiFi.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include "config/MqttDefaults.h"
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

    String willTopic =
        String(MQTT_ROOT_TOPIC) +
        "/devices/" +
        nodeId +
        "/status";

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

        // Assina comandos diretos ao device
        String cmdTopic =
            String(MQTT_ROOT_TOPIC) +
            "/devices/" +
            nodeId +
            "/#";

        _mqtt.subscribe(cmdTopic.c_str());

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

/*
void MqttService::onMessage(char* topic,
                            byte* payload,
                            unsigned int length) {

    String msg;

    for (unsigned int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }

    LogService::info("MQTT RX: " + String(topic) + " -> " + msg);
}
*/

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

    // Verifica se é tópico de comando
    const String nodeId = NodeIdentity::id();
    String cmdPrefix =
        String(MQTT_ROOT_TOPIC) +
        "/" + nodeId + "/";

    if (!topicStr.startsWith(cmdPrefix)) {
        return; // não é comando do device
    }

    if (_commandCallback == nullptr) {
        return; // usuário não registrou callback
    }

    // Parse JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, msg);

    if (error) {
        LogService::warn("JSON inválido");
        return;
    }

    String action = doc["action"] | "";
    String value  = doc["value"]  | "";

    _commandCallback(action, value, doc);

    String base =
    String(MQTT_ROOT_TOPIC) +
    "/devices/" +
    nodeId +
    "/";

    if (!topicStr.startsWith(base)) return;

    String channel = topicStr.substring(base.length());

    if (channel == "cmd") {
        handleCommand(msg);
    }
    else if (channel == "config") {
        handleConfig(msg);
    }
    else if (channel == "ota") {
        handleOta(msg);
    }
}

void MqttService::publishHello() {

    const String nodeId = NodeIdentity::id();

    String topic =
        String(MQTT_ROOT_TOPIC) +
        "/devices/" +
        nodeId +
        "/status";

    String payload = "{";
    payload += "\"chip\":\"" + nodeId + "\",";
    payload += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
    payload += "}";

    _mqtt.publish(topic.c_str(), payload.c_str(), true);
}

void MqttService::publishData(const String& payload, bool retain) {

    if (!_mqtt.connected()) return;

    String topic =
        String(MQTT_ROOT_TOPIC) +
        "/devices/" +
        NodeIdentity::id() +
        "/data";

    _mqtt.publish(topic.c_str(), payload.c_str(), retain);
}

void MqttService::publishSerial(const String& message) {

    if (!_mqtt.connected()) return;

    String topic =
        String(MQTT_ROOT_TOPIC) +
        "/serial/" +
        NodeIdentity::id();

    _mqtt.publish(topic.c_str(), message.c_str(), false);
}

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
