#include "MyLibereCore.h"

void MyLibereCore::begin() {
    _wifi.begin();
    _mqtt.begin();
    _ota.begin();
    OtaHttpService::begin();

}

void MyLibereCore::loop() {
    _wifi.loop();
    _mqtt.loop();
    _ota.loop();
    OtaHttpService::loop();

}

MqttService& MyLibereCore::mqtt() {
    return _mqtt;
}

