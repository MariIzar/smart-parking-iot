#pragma once
// MODELO: copiar como secrets.h e preencher somente localmente.
// Para broker sem TLS na porta 1883, usar apenas em rede local confiavel.
// Brokers de nuvem normalmente exigem TLS: veja README antes de usar.
const char* WIFI_SSID = "REDE_WIFI";
const char* WIFI_PASSWORD = "SENHA_WIFI";
const char* MQTT_HOST = "192.168.1.10";
const unsigned int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "";
const char* MQTT_PASSWORD = "";
