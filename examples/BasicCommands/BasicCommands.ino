#include <MyLibere.h>

/*
===========================================================
Exemplo: Recebimento de comandos MQTT
===========================================================

Objetivo:
Demonstrar como registrar um callback de comando e
executar ações locais no dispositivo.

Fluxo:
1) Device conecta ao Wi-Fi
2) Conecta ao broker MQTT
3) Aguarda comandos no tópico do device
4) Executa ação local
5) Publica resposta no MQTT

Comando esperado (JSON):
{
    "action": "led",
    "value": "on"
}
===========================================================
*/

// ----------------------------------------------------------
// Definição de hardware
// ----------------------------------------------------------

// ESP8266 → LED interno funciona com lógica invertida
// LOW  = LED ligado
// HIGH = LED desligado
const int LED_PIN = 2;


// ----------------------------------------------------------
// Callback de comando MQTT
// ----------------------------------------------------------
// Essa função será chamada automaticamente sempre que
// o device receber um comando no tópico correto.
//
// action → tipo de ação (ex: "led")
// value  → valor da ação (ex: "on")
// doc    → JSON completo recebido (caso queira usar mais campos)
// ----------------------------------------------------------
void handleCommand(
    const String& action,
    const String& value,
    JsonDocument& doc
) {

    // Verifica se o comando é para controlar o LED
    if (action == "led") {

        if (value == "on") {

            digitalWrite(LED_PIN, LOW); // Liga LED

            // Publica retorno informando sucesso
            MyLibere::mqttInfo("LED ON");
        }
        else if (value == "off") {

            digitalWrite(LED_PIN, HIGH); // Desliga LED

            MyLibere::mqttInfo("LED OFF");
        }
        else {
            // Caso receba valor inválido
            MyLibere::logWarn("Valor inválido para LED");
        }
    }
}


// ----------------------------------------------------------
// Setup
// ----------------------------------------------------------
void setup() {

    Serial.begin(115200);

    // Inicializa pino do LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); // começa desligado

    // Inicializa MyLibere
    // Responsável por:
    // ✔ Wi-Fi
    // ✔ MQTT
    // ✔ Serviços internos
    MyLibere::begin();

    // Registra a função de callback para comandos MQTT
    MyLibere::mqtt().onCommand(handleCommand);

    MyLibere::logInfo("Sistema iniciado");
}


// ----------------------------------------------------------
// Loop principal
// ----------------------------------------------------------
// Deve ser chamado continuamente.
// Responsável por:
// ✔ Manter conexão MQTT
// ✔ Processar mensagens recebidas
// ✔ Executar tarefas internas da biblioteca
// ----------------------------------------------------------
void loop() {

    MyLibere::loop();
}
