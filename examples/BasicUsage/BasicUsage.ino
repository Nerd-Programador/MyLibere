#include <MyLibere.h>

/*
Exemplo básico de uso da biblioteca MyLibere

Responsabilidade:
- Inicializa toda a stack IoT (WiFi, MQTT, OTA, Logs)
- Executa o loop principal não-bloqueante
*/

void setup() {

    // Inicializa serial para debug
    Serial.begin(115200);
    delay(100); // pequeno tempo só para estabilizar serial (ok usar aqui)

    // Inicializa framework
    MyLibere::begin();
}

void loop() {

    // Loop principal do framework (não bloqueante)
    MyLibere::loop();
}
