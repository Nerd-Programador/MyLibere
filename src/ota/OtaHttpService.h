#pragma once
#include <Arduino.h>

class OtaHttpService {
public:
    static void begin();
    static void loop();

private:
    static void checkForUpdate();
    static bool downloadAndUpdate(const String& url);
};
