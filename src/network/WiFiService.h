#pragma once
#include <Arduino.h>

class WiFiService {
public:
    static void begin();
    static void loop();

    static bool isConnected();
    static String ip();

private:
    static void connect();

    static unsigned long lastAttempt;
    static unsigned long startTime;
    static bool connecting;
};
