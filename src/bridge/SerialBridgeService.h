#pragma once
#include <Arduino.h>

class SerialBridgeService {
public:
    static void begin();
    static void log(const String& message,
                    const String& level = "info");
};
