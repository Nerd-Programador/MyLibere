#pragma once
#include <Arduino.h>

class NodeIdentity {
public:
    static void begin();
    static const String& id();

private:
    static String nodeId;
};
