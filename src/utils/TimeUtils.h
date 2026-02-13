#pragma once
#include <Arduino.h>

namespace TimeUtils {

    // Retorna millis atual
    inline unsigned long now() {
        return millis();
    }

    // Verifica se intervalo passou
    inline bool elapsed(unsigned long &last, unsigned long interval) {
        if (millis() - last >= interval) {
            return true;
        }
        return false;
    }

    // Reseta contador
    inline void reset(unsigned long &last) {
        last = millis();
    }
    
    inline bool timeout(unsigned long start, unsigned long limit) {
        return (millis() - start) >= limit;
    }
}

