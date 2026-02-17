#include <MyLibere.h>
#include "UserConfig.h"

/*
===========================================================
Exemplo Oficial MyLibere – MQTT Completo (Serial-like)
===========================================================

Demonstra:

✔ Inicialização da MyLibere
✔ Envio periódico de dados (não bloqueante com millis)
✔ Log via MQTT usando máscara estilo Serial (Mqtt.info)
✔ Estrutura limpa e organizada
✔ Compatível com ESP32 e ESP8266

Tópicos (padrão MyLibere):

servermaster/<NodeID>/<ENV>/data
servermaster/<NodeID>/<ENV>/serial
servermaster/<NodeID>/<ENV>/status
servermaster/<NodeID>/<ENV>/cmd/#

Dica:
- <ENV> vem de DEVICE_ENVIRONMENT (ex: "cooksmart")
===========================================================
*/

// ==========================================================
// CONFIGURAÇÕES DO EXEMPLO
// ==========================================================

const unsigned long SEND_INTERVAL = 5000;  // intervalo de envio (ms)
unsigned long lastSend = 0;                // controle de tempo (millis)

// ==========================================================
// SETUP
// ==========================================================

void setup() {
    Serial.begin(115200);

    // Responsabilidade da MyLibere:
    // - gerar NodeID
    // - conectar WiFi
    // - conectar MQTT
    // - manter serviços internos
    MyLibere::begin();

    Log.info("BasicMQTT iniciado");
    Mqtt.event("boot");
}

// ==========================================================
// LOOP PRINCIPAL
// ==========================================================

void loop() {
    MyLibere::loop();

    const unsigned long now = millis();

    if (now - lastSend >= SEND_INTERVAL) {
        lastSend = now;

        // Payload JSON simples (leve e fácil de debugar)
        String payload = "{";
        payload += "\"temp\":28.5,";
        payload += "\"umidade\":52";
        payload += "}";

        // Publica em: servermaster/<NodeID>/<ENV>/data
        Mqtt.data(payload, false);

        // Log humano em: servermaster/<NodeID>/<ENV>/serial
        Mqtt.info("Enviando dados MQTT");
    }
}
