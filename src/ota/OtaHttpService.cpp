#include "OtaHttpService.h"
#include "config/OtaDefaults.h"
#include "config/EnvironmentConfig.h"
#include "network/WiFiService.h"
#include "log/LogService.h"

#if defined(ESP8266)
    #include <ESP8266httpUpdate.h>
#elif defined(ESP32)
    #include <HTTPUpdate.h>
    #include <WiFiClient.h>
#endif

static unsigned long lastCheck = 0;

void OtaHttpService::begin() {
    lastCheck = 0;
}

void OtaHttpService::loop() {

#if OTA_HTTP_ENABLED

    if (!WiFiService::isConnected()) return;

    if (millis() - lastCheck < OTA_HTTP_CHECK_INTERVAL) return;

    lastCheck = millis();

    checkForUpdate();

#endif
}

void OtaHttpService::checkForUpdate() {

    String url = String(OTA_HTTP_BASE_URL) + "/" +
                        DEVICE_ENVIRONMENT + "/firmware.bin";

    LogService::info("Verificando OTA HTTP: " + url);

    downloadAndUpdate(url);
}

bool OtaHttpService::downloadAndUpdate(const String& url) {

#if defined(ESP8266)

    WiFiClient client;
    t_httpUpdate_return result = ESPhttpUpdate.update(client, url);

    switch (result) {
        case HTTP_UPDATE_FAILED:
            LogService::error("HTTP OTA falhou");
            return false;

        case HTTP_UPDATE_NO_UPDATES:
            LogService::info("Sem atualizações");
            return false;

        case HTTP_UPDATE_OK:
            LogService::info("Atualização aplicada. Reiniciando...");
            return true;
    }

#elif defined(ESP32)

    WiFiClient client;
    HTTPUpdate httpUpdate;

    t_httpUpdate_return result = httpUpdate.update(client, url);

    if (result == HTTP_UPDATE_OK) {
        LogService::info("Atualização aplicada. Reiniciando...");
        return true;
    }

    LogService::warn("HTTP OTA falhou");
    return false;

#endif

    return false;
}

