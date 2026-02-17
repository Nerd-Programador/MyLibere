#include <MyLibere.h>
#include "UserConfig.h"

/*
===========================================================
Exemplo: Recebimento de comandos MQTT (Serial-like)
===========================================================

Objetivo:
Demonstrar como registrar callback de comando e
executar ações locais no dispositivo.

Comando esperado (JSON):
{
    "action": "led",
    "value": "on"
}

Onde chega:
servermaster/<NodeID>/<ENV>/cmd/do
===========================================================
*/

// ----------------------------------------------------------
// Definição de hardware (LED)
// ----------------------------------------------------------
// ESP8266 (NodeMCU): LED interno geralmente é GPIO2 e invertido (LOW liga).
// ESP32: depende da placa; muitas DevKit nem têm LED fixo no GPIO2.
// Deixei um padrão seguro: se não tiver LED, você troca o LED_PIN.
#if defined(ESP8266)
    const int LED_PIN = 2;
    const bool LED_INVERTED = true;
#else
    const int LED_PIN = 2;      // ajuste se sua ESP32 usar outro pino
    const bool LED_INVERTED = false;
#endif

void setLed(bool on) {
    // Responsabilidade:
    // - abstrair a inversão (ESP8266) e deixar o código de comando limpo.
    if (LED_INVERTED) {
        digitalWrite(LED_PIN, on ? LOW : HIGH);
    } else {
        digitalWrite(LED_PIN, on ? HIGH : LOW);
    }
}

// ----------------------------------------------------------
// Callback de comando MQTT
// ----------------------------------------------------------
void handleCommand(const String& action, const String& value, JsonDocument& doc) {
    (void)doc; // não usado agora, mas deixado para expansão futura

    if (action != "led") return;

    if (value == "on") {
        setLed(true);
        Mqtt.info("LED ON");
        Log.info("Comando LED ON aplicado");
    }
    else if (value == "off") {
        setLed(false);
        Mqtt.info("LED OFF");
        Log.info("Comando LED OFF aplicado");
    }
    else {
        Log.warn("Valor inválido para LED: " + value);
        Mqtt.warn("Valor inválido para LED");
    }
}

// ----------------------------------------------------------
// Setup
// ----------------------------------------------------------
void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    setLed(false);

    MyLibere::begin();

    // Registra callback de comandos
    MyLibere::mqtt().onCommand(handleCommand);

    Log.info("Sistema iniciado");
    Mqtt.event("ready");
}

// ----------------------------------------------------------
// Loop
// ----------------------------------------------------------
void loop() {
    MyLibere::loop();
}
