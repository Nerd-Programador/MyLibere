#include "MyLibere.h"
#include "core/NodeIdentity.h"
#include "log/LogService.h"

MyLibereCore MyLibere::core;

void MyLibere::begin() {

    Serial.println("========== BOOT ==========");

    NodeIdentity::begin();
    LogService::begin();

    Serial.print("Node ID: ");
    Serial.println(NodeIdentity::id());

    core.begin();   // SOMENTE isso
}

void MyLibere::loop() {
    core.loop();
    
    #if defined(ESP8266)
        yield();
    #endif

}
