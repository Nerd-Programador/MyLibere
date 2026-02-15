# CHANGELOG

## [1.0.0] - 2026-02-12
### Added
- WiFiService não bloqueante
- MQTT com LWT
- OTA IDE
- OTA HTTP por ambiente
- LogService com buffer
- NodeIdentity automática

### Changed
- Removido ambiente fixo da biblioteca
- Configuração via UserConfig.h

### Fixed
- Correção reconexão MQTT

## [1.1.0] - Initial Stable Version
- Estrutura modular consolidada
- WiFiService não bloqueante
- MqttService com reconnect automático
- OTA IDE funcional
- OTA HTTP estruturado por ambiente
- SerialBridge integrado ao LogService
- Callback oficial de comandos MQTT
- Padrão oficial de tópicos (/cmd, /config, /ota)
