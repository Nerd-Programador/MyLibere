#pragma once

#include "network/WiFiService.h"
#include "mqtt/MqttService.h"
#include "ota/OtaIdeService.h"
#include "ota/OtaHttpService.h"

class MyLibereCore {
public:
    void begin();
    void loop();

    MqttService& mqtt();

private:
    WiFiService _wifi;
    MqttService _mqtt;
    OtaIdeService _ota;
    OtaHttpService _otaHttp;
};
