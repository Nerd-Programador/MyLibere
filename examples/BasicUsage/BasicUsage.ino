#include <MyLibere.h>

/*
===========================================================
Exemplo Oficial MyLibere – Inicialização Básica
===========================================================

Responsabilidade:

✔ Inicializa WiFi
✔ Inicializa MQTT
✔ Inicializa sistema de logs
✔ Inicializa OTA (quando habilitado)
✔ Mantém o loop principal não bloqueante

Este é o menor exemplo funcional da biblioteca.
===========================================================
*/


// ==========================================================
// SETUP
// ==========================================================

void setup() {

    // Inicializa comunicação serial para debug local
    Serial.begin(115200);

    // Inicializa toda a stack IoT
    // Internamente:
    // - Conecta no WiFi
    // - Configura identidade do node
    // - Inicializa MQTT
    // - Prepara sistema OTA
    MyLibere::begin();
}


// ==========================================================
// LOOP PRINCIPAL
// ==========================================================

void loop() {

    // Mantém todos os serviços internos ativos
    // (WiFi, MQTT, reconexões, OTA futuro, etc.)
    // Totalmente não bloqueante
    MyLibere::loop();
}
