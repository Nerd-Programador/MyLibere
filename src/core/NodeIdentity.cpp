#include "NodeIdentity.h"

#if defined(ESP32)
    #include <WiFi.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#endif

String NodeIdentity::nodeId;

void NodeIdentity::begin() {
    #if defined(ESP32)
        uint64_t mac = ESP.getEfuseMac();
        char buf[13];
        snprintf(buf, sizeof(buf), "%06X", (uint32_t)(mac & 0xFFFFFF));
        nodeId = "ESP-" + String(buf);

    #elif defined(ESP8266)
        uint32_t chipId = ESP.getChipId();
        char buf[7];
        snprintf(buf, sizeof(buf), "%06X", chipId);
        nodeId = "ESP-" + String(buf);
    #endif
}

const String& NodeIdentity::id() {
    return nodeId;
}