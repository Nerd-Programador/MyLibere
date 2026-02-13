#pragma once

/* =======================
    IDENTIDADE DO DEVICE
======================= */
//#define DEVICE_ENVIRONMENT "cooksmart"

/* =======================
    MQTT - Raspberry Pi
======================= */
/*#define MQTT_ENABLED        true
#define MQTT_BROKER         "192.168.15.60"
#define MQTT_PORT           1883
#define MQTT_USER           "esp_iot"
#define MQTT_PASS           "IOT"
#define MQTT_ROOT_TOPIC     "servermaster"*/

/* =======================
    OTA HTTP
======================= */
#define OTA_HTTP_ENABLED true
#define OTA_HTTP_BASE_URL    "http://192.168.15.60:5000/firmware"   // Base da sua Pi
//#define OTA_HTTP_ENVIRONMENT "cooksmart"    // Ambiente (pasta dentro do pendrive)
//#define FIRMWARE_VERSION     "1.0.0"    // Versão atual do firmware
#define OTA_HTTP_CHECK_INTERVAL 60000 // Intervalo de verificação (60s)

/* =======================
    FIREBASE (via bridge)
======================= */
#define FIREBASE_ENABLED    true
#define FIREBASE_DB_URL     "https://servermaster-iot-default-rtdb.firebaseio.com/"

/* =======================
    LOGGING
======================= */
#define LOG_BUFFER_SIZE    50
#define LOG_MQTT_ENABLED   true
#define LOG_SERIAL_ENABLED true


#define SERIAL_BRIDGE_ENABLED true
