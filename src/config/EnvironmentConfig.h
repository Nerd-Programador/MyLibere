#pragma once
#include <Arduino.h>

/*
    A biblioteca NÃO deve quebrar sozinha.
    Se o projeto não definir UserConfig.h,
    usamos valores padrão seguros.
*/

#if __has_include(<UserConfig.h>)
    #include <UserConfig.h>
#endif

// Se o usuário não definiu, usa placeholder
#ifndef DEVICE_ENVIRONMENT
    #define DEVICE_ENVIRONMENT "undefined"
#endif

#ifndef FIRMWARE_VERSION
    #define FIRMWARE_VERSION "0.0.0-dev"
#endif
