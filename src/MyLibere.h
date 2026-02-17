#pragma once

#include "core/MyLibereCore.h"

class MyLibere {
public:
    static void begin();
    static void loop();

    static MqttService& mqtt();

    /* =======================
        Máscara de Log (Serial-like)
        Uso: MyLibere::log().info("...");
       ======================= */
    class LogMask {
    public:
        void info(const String& msg);
        void warn(const String& msg);
        void error(const String& msg);

        // Estilo Serial
        void print(const String& msg);
        void println(const String& msg);
        void println();
    };

    static LogMask& log();

    // Atalhos MQTT
    static void mqttInfo(const String& msg);
    static void mqttCmd(const String& action, const String& value);
    static void mqttData(const String& json);

    // Atalhos Log
    static void logInfo(const String& msg);
    static void logWarn(const String& msg);
    static void logError(const String& msg);

private:
    static MyLibereCore core;
    static LogMask _log;
};

/* =====================================================================
   Atalhos globais (Serial-like)

   Responsabilidade:
   - Entregar uma API curtinha e intuitiva, no estilo do Arduino Serial.
   - Evitar que você precise lembrar "MyLibere::mqtt().cmd(...)" toda hora.

   Uso:
     Mqtt.cmd("luz", true);
     Mqtt.info("Recebido");
     Log.info("Luz ligada");

   Observação:
   - Estes objetos só encaminham chamadas. A lógica real continua em
     MqttService e LogService.
   ===================================================================== */

// Constantes opcionais para ficar natural: Mqtt.cmd("luz", ON)
// (sem quebrar caso algum core já defina ON/OFF)
#ifndef ON
constexpr bool ON = true;
#endif
#ifndef OFF
constexpr bool OFF = false;
#endif

struct MyLibereMqttShortcut {
    /* -----------------------
       Publicação genérica
       -----------------------
       Publica em: servermaster/<NodeID>/<ENV>/<subtopic>
       Ex: Mqtt.pub("telemetry", "{...}")
    */
    bool pub(const char* subtopic, const char* payload, bool retain = false) {
        return MyLibere::mqtt().pub(String(subtopic), String(payload), retain);
    }

    /* -----------------------
       Canais mais usados
       ----------------------- */
    bool status(const char* payload, bool retain = true) {
        return MyLibere::mqtt().status(String(payload), retain);
    }

    bool data(const char* payload, bool retain = false) {
        return MyLibere::mqtt().data(String(payload), retain);
    }

    bool tele(const char* payload, bool retain = false) {
        return MyLibere::mqtt().tele(String(payload), retain);
    }

    bool event(const char* type, const char* payloadJson = "{}") {
        return MyLibere::mqtt().event(String(type), String(payloadJson));
    }

    /* -----------------------
       Logs via MQTT (canal serial)
       ----------------------- */
    bool info(const char* msg)  { return MyLibere::mqtt().info(String(msg)); }
    bool warn(const char* msg)  { return MyLibere::mqtt().warn(String(msg)); }
    bool error(const char* msg) { return MyLibere::mqtt().error(String(msg)); }

    // Estilo Serial
    bool print(const char* msg)   { return MyLibere::mqtt().print(String(msg)); }
    bool println(const char* msg) { return MyLibere::mqtt().println(String(msg)); }
    bool println()                { return MyLibere::mqtt().println(); }

    /* -----------------------
       Comandos (cmd)
       -----------------------
       Objetivo:
       - Aceitar tipos comuns sem você ficar convertendo pra String.
       - bool vira "ON" / "OFF" (mais humano no debug e no broker).
    */
    bool cmd(const char* action, const char* value) {
        return MyLibere::mqtt().cmd(String(action), String(value));
    }

    bool cmd(const char* action, bool value) {
        return MyLibere::mqtt().cmd(String(action), value);
    }

    bool cmd(const char* action, int value) {
        return MyLibere::mqtt().cmd(String(action), value);
    }

    bool cmd(const char* action, float value, uint8_t decimals = 2) {
        return MyLibere::mqtt().cmd(String(action), value, decimals);
    }
};

struct MyLibereLogShortcut {
    /* -----------------------
       Log com níveis
       -----------------------
       Encaminha para LogService via MyLibere::log()
    */
    void info(const char* msg)  { MyLibere::log().info(String(msg)); }
    void warn(const char* msg)  { MyLibere::log().warn(String(msg)); }
    void error(const char* msg) { MyLibere::log().error(String(msg)); }

    // Estilo Serial
    void print(const char* msg)   { MyLibere::log().print(String(msg)); }
    void println(const char* msg) { MyLibere::log().println(String(msg)); }
    void println()                { MyLibere::log().println(); }
};

// Objetos globais (como Serial)
extern MyLibereMqttShortcut Mqtt;
extern MyLibereLogShortcut  Log;
