#include "SerialBridgeService.h"
#include "core/NodeIdentity.h"
#include "mqtt/MqttService.h"
#include "config/MyLibereConfig.h"

#if defined(ESP8266)
    #include <ESP8266HTTPClient.h>
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <HTTPClient.h>
    #include <WiFi.h>
#endif

void SerialBridgeService::begin() {
    // reservado para futuras configs
}

void SerialBridgeService::log(const String& message,
                            const String& level) {

#if SERIAL_MQTT_ENABLED
    MqttService::publishTo(
        "serial/" + NodeIdentity::id(),
        message
    );
#endif


#if SERIAL_HTTP_ENABLED
    WiFiClient client;
    HTTPClient http;

    String url = "http://192.168.15.60:5000/api/serial/log";

    if (http.begin(client, url)) {

        http.addHeader("Content-Type", "application/json");

        String payload = "{";
        payload += "\"source\":\"" + NodeIdentity::id() + "\",";
        payload += "\"message\":\"" + message + "\",";
        payload += "\"type\":\"" + level + "\"";
        payload += "}";

        http.POST(payload);
        http.end();
    }
#endif
}
