#include <MyLibere.h>

/*
===========================================================
Exemplo Oficial MyLibere – MQTT Completo
===========================================================

Demonstra:

✔ Inicialização da lib
✔ Envio periódico de dados (não bloqueante)
✔ Publicação de log MQTT
✔ Estrutura limpa e organizada
✔ Compatível com ESP32 e ESP8266

Tópicos utilizados automaticamente:

/devices/{nodeId}/data
/serial/{nodeId}
/{nodeId}/cmd
===========================================================
*/


// ==========================================================
// CONFIGURAÇÕES DO EXEMPLO
// ==========================================================

// Intervalo de envio de dados (não bloqueante)
const unsigned long SEND_INTERVAL = 5000;

// Controle interno de tempo (millis)
unsigned long lastSend = 0;


// ==========================================================
// SETUP
// ==========================================================

void setup() {

    // Inicializa Serial
    Serial.begin(115200);

    // Inicializa framework MyLibere
    // Responsável por:
    // - Identidade do node
    // - WiFi
    // - MQTT
    // - Logs
    MyLibere::begin();
}


// ==========================================================
// LOOP PRINCIPAL
// ==========================================================

void loop() {

    // Mantém serviços internos funcionando
    // (WiFi, MQTT, OTA futuro, etc.)
    MyLibere::loop();

    unsigned long now = millis();

    // ======================================================
    // ENVIO PERIÓDICO DE DADOS (NÃO BLOQUEANTE)
    // ======================================================

    if (now - lastSend >= SEND_INTERVAL) {

        lastSend = now;

        // ---------------------------------------------
        // Montagem de payload JSON simples e leve
        // ---------------------------------------------

        String payload = "{";
        payload += "\"temp\":28.5,";
        payload += "\"umidade\":52";
        payload += "}";

        // Publica dados no tópico oficial:
        // /devices/{nodeId}/data
        MyLibere::mqtt().publishData(payload, false);

        // Publica log no tópico serial:
        // /serial/{nodeId}
        MyLibere::mqtt().publishSerial("Enviando dados MQTT");
    }
}
