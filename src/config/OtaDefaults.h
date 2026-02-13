#pragma once

/* =======================
    OTA IDE
======================= */
#define OTA_IDE_ENABLED         true
#define OTA_IDE_PASSWORD        "otapass"

/* =======================
    OTA HTTP
======================= */
#define OTA_HTTP_ENABLED true
#define OTA_HTTP_BASE_URL    "http://192.168.15.60:5000/firmware"   // Base da sua Pi
//#define OTA_HTTP_ENVIRONMENT "cooksmart"    // Ambiente (pasta dentro do pendrive)
//#define FIRMWARE_VERSION     "1.0.0"    // Versão atual do firmware
#define OTA_HTTP_CHECK_INTERVAL 60000 // Intervalo de verificação (60s)
