#include "WiFiService.h"

#if defined(ESP32)
    #include <WiFi.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#endif

#include "config/WiFiDefaults.h"
#include "utils/TimeUtils.h"

/* =========================
OVERRIDE DE CONFIG
========================= */
#ifndef WIFI_SSID
    #define WIFI_SSID DEFAULT_WIFI_SSID
#endif

#ifndef WIFI_PASS
    #define WIFI_PASS DEFAULT_WIFI_PASS
#endif

/* =========================
VARIÁVEIS ESTÁTICAS
========================= */
unsigned long WiFiService::lastAttempt = 0;
unsigned long WiFiService::startTime   = 0;
bool WiFiService::connecting           = false;

/* =========================
PUBLIC
========================= */
void WiFiService::begin() {

    Serial.println("Iniciando WiFiService...");

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(false);
    WiFi.persistent(false);

    Serial.println("Conectando Wi-Fi...");

    lastAttempt = 0;
    startTime = millis();
    connecting = true;

    connect();   // <-- força primeira tentativa imediatamente
}

void WiFiService::loop() {

    wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED) {

        if (connecting) {
            Serial.print("Conectado a rede: ");
            Serial.println(WIFI_SSID);
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
        }

        connecting = false;
        return;
    }

    if (!connecting) return;

    if (TimeUtils::elapsed(lastAttempt, WIFI_CONNECT_INTERVAL)) {
        TimeUtils::reset(lastAttempt);
        Serial.println("Tentando conexão...");
        connect();
    }

    if (TimeUtils::timeout(startTime, WIFI_CONNECT_TIMEOUT)) {
        Serial.println("Falha ao conectar Wi-Fi");
        connecting = false;
    }
}

bool WiFiService::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiService::ip() {
    return isConnected() ? WiFi.localIP().toString() : String("-");
}

/* =========================
PRIVATE
========================= */
void WiFiService::connect() {
    WiFi.disconnect(); // garante limpeza de estado
    /*WiFi.begin(WIFI_SSID, WIFI_PASS);*/
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }

    Serial.print("SSID compilado: ");
    Serial.println(WIFI_SSID);
    Serial.print("PASS compilado: ");
    Serial.println(WIFI_PASS);

}
