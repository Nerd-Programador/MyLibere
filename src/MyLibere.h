#pragma once
#include "core/MyLibereCore.h"

class MyLibere {
public:
    static void begin();
    static void loop();

private:
    static MyLibereCore core;
};
