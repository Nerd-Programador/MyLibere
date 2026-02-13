#include "OtaIdeService.h"
#include "config/OtaDefaults.h"

#if defined(ESP32)
    #include <WiFi.h>
    #include <ArduinoOTA.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ArduinoOTA.h>
#endif

#include "network/WiFiService.h"
#include "log/LogService.h"
#include "core/NodeIdentity.h"

/* =========================
ESTADO INTERNO
========================= */
static bool otaStarted = false;

/* =========================
PUBLIC
========================= */
void OtaIdeService::begin() {
#if OTA_IDE_ENABLED

    // Define hostname único baseado na identidade do nó
    ArduinoOTA.setHostname(NodeIdentity::id().c_str());

    ArduinoOTA.setPassword(OTA_IDE_PASSWORD);

    ArduinoOTA.onStart([]() {
        String type = (ArduinoOTA.getCommand() == U_FLASH) ? "firmware" : "filesystem";
        LogService::warn("OTA iniciado: " + type);
    });


    ArduinoOTA.onEnd([]() {
        LogService::info("OTA finalizado");
    });

    ArduinoOTA.onError([](ota_error_t error) {
        LogService::error("OTA erro: " + String(error));
    });

#endif
}

void OtaIdeService::loop() {
#if OTA_IDE_ENABLED

    // Se WiFi caiu, resetar estado
    if (!WiFiService::isConnected()) {
        otaStarted = false;
        return;
    }

    // Inicia OTA apenas uma vez após conexão WiFi
    if (!otaStarted) {
        ArduinoOTA.begin();
        otaStarted = true;

        LogService::info(
            "OTA pronto: " + NodeIdentity::id()
        );
    }

    ArduinoOTA.handle();

#endif
}
