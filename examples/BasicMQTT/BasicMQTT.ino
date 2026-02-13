#include <MyLibere.h>

/*
Exemplo MQTT completo

Demonstra:

✔ Publicação de dados
✔ Publicação de log serial
✔ Recebimento de comandos
✔ Uso não bloqueante com millis()
*/

unsigned long lastSend = 0;
const unsigned long interval = 5000;

void setup() {

    Serial.begin(115200);
    delay(100);

    MyLibere::begin();
}

void loop() {

    MyLibere::loop();

    unsigned long now = millis();

    // Envio periódico de dados
    if (now - lastSend >= interval) {

        lastSend = now;

        // Payload JSON manual (simples e leve)
        String payload = "{";
        payload += "\"temp\":28.5,";
        payload += "\"umidade\":52";
        payload += "}";

        MyLibere::mqtt().publishData(payload, false);

        MyLibere::mqtt().publishSerial("Enviando dados MQTT");
    }
}
