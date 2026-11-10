/*
 * Reconstrucao proposta do modulo de vagas em ESP32 (NAO codigo recuperado).
 * Quatro entradas digitais de sensores de presenca + MQTT com TLS opcional.
 * Dependencias: PubSubClient (instalar pelo Gerenciador de Bibliotecas).
 * Os pinos e o tipo de sensor sao HIPOTESES. Validar antes de gravar.
 * Nunca publicar credenciais reais.
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"  // Copiar secrets.example.h -> secrets.h (arquivo ignorado pelo Git)

// PINOS DE EXEMPLO para ESP32 DevKit; confira modelo e fiaçao.
const uint8_t PIN_VAGAS[4] = {16, 17, 18, 19};
const bool SENSOR_OCUPADO_EM_LOW = true;  // Exemplo: sensores digitais ativos em LOW
const bool USAR_PULLUP_INTERNO = true;   // Confirmar saida do sensor antes de ativar
const unsigned long DEBOUNCE_MS = 250;
const unsigned long WIFI_RETRY_MS = 10000;
const unsigned long MQTT_RETRY_MS = 5000;
const unsigned long PUBLICAR_CADA_MS = 30000;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
bool ocupado[4] = {false, false, false, false};
bool leituraAnterior[4] = {false, false, false, false};
unsigned long mudouEm[4] = {0, 0, 0, 0};
unsigned long ultimaTentativaWifi = 0;
unsigned long ultimaTentativaMqtt = 0;
unsigned long ultimaPublicacao = 0;
bool publicarPendente = true;

bool lerOcupacao(uint8_t i) {
  int nivel = digitalRead(PIN_VAGAS[i]);
  return SENSOR_OCUPADO_EM_LOW ? (nivel == LOW) : (nivel == HIGH);
}

void atualizarSensores(unsigned long agora) {
  for (uint8_t i = 0; i < 4; ++i) {
    bool leitura = lerOcupacao(i);
    if (leitura != leituraAnterior[i]) {
      leituraAnterior[i] = leitura;
      mudouEm[i] = agora;
    }
    if ((agora - mudouEm[i] >= DEBOUNCE_MS) && (ocupado[i] != leituraAnterior[i])) {
      ocupado[i] = leituraAnterior[i];
      publicarPendente = true;
      Serial.printf("Vaga %u: %s\n", i + 1, ocupado[i] ? "OCUPADA" : "LIVRE");
    }
  }
}

void publicarEstado() {
  if (!mqtt.connected()) return;
  uint8_t livres = 0;
  for (uint8_t i = 0; i < 4; i++) {
    char topico[64];
    snprintf(topico, sizeof(topico), "estacionamento/vagas/%u", i + 1);
    mqtt.publish(topico, ocupado[i] ? "ocupada" : "livre", true);
    if (!ocupado[i]) livres++;
  }
  char texto[8];
  snprintf(texto, sizeof(texto), "%u", livres);
  mqtt.publish("estacionamento/vagas_livres", texto, true);
  mqtt.publish("estacionamento/lotado", livres == 0 ? "sim" : "nao", true);
  publicarPendente = false;
  ultimaPublicacao = millis();
}

void conectarWifi(unsigned long agora) {
  if (WiFi.status() == WL_CONNECTED) return;
  if (ultimaTentativaWifi && agora - ultimaTentativaWifi < WIFI_RETRY_MS) return;
  ultimaTentativaWifi = agora;
  Serial.println("Tentando conectar Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void conectarMqtt(unsigned long agora) {
  if (WiFi.status() != WL_CONNECTED || mqtt.connected()) return;
  if (ultimaTentativaMqtt && agora - ultimaTentativaMqtt < MQTT_RETRY_MS) return;
  ultimaTentativaMqtt = agora;
  String clientId = "estacionamento-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  bool ok = (MQTT_USERNAME[0] != '\0')
      ? mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)
      : mqtt.connect(clientId.c_str());
  if (ok) {
    Serial.println("MQTT conectado");
    publicarPendente = true;
  } else {
    Serial.printf("MQTT falhou: %d\n", mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < 4; ++i) {
    pinMode(PIN_VAGAS[i], USAR_PULLUP_INTERNO ? INPUT_PULLUP : INPUT);
    ocupado[i] = leituraAnterior[i] = lerOcupacao(i);
    mudouEm[i] = millis();
  }
  WiFi.mode(WIFI_STA);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setBufferSize(256);
  // Para servicos em 8883 (TLS), substituir WiFiClient por WiFiClientSecure,
  // configurar verificacao de certificado e validar o broker antes de usar.
}

void loop() {
  unsigned long agora = millis();
  atualizarSensores(agora);
  conectarWifi(agora);
  conectarMqtt(agora);
  if (mqtt.connected()) mqtt.loop();
  if (publicarPendente || agora - ultimaPublicacao >= PUBLICAR_CADA_MS) {
    publicarEstado();
  }
  delay(15);
}
