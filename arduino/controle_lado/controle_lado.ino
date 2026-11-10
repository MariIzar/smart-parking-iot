/*
 * Reconstrucao proposta do Estacionamento Inteligente (nao e fonte original).
 * Duas placas Arduino Uno executam este mesmo sketch.
 * Cada uma gerencia 1 cancela, 1 HC-SR04, 1 LDR e 2 postes de LED.
 * PINOS, polaridade e tempos sao EXEMPLOS: confirme a fiacao ANTES de gravar!
 * Biblioteca Servo incluida no Arduino IDE.
 */
#include <Servo.h>

// ===== CONFIGURACAO: CONFERIR COM A MAQUETE =====
const uint8_t PIN_TRIG = 7;          // EXEMPLO
const uint8_t PIN_ECHO = 8;          // EXEMPLO
const uint8_t PIN_SERVO = 9;         // EXEMPLO
const uint8_t PIN_POSTE_1 = 4;       // EXEMPLO (usar resistor/driver adequado)
const uint8_t PIN_POSTE_2 = 5;       // EXEMPLO
const uint8_t PIN_LDR = A0;          // EXEMPLO, LDR em divisor de tensao

const char *NOME_LADO = "SAIDA";     // Alterar para "ENTRADA" na outra placa
const bool ESCURO_QUANDO_LEITURA_BAIXA = true; // Depende de como o divisor foi ligado
const int LIMIAR_ESCURO = 420;       // EXEMPLO, calibrar no local
const int HISTERESE_LDR = 35;
const bool LED_ATIVO_EM_HIGH = true; // Trocar se o circuito usar transistor invertido
const int SERVO_ABERTO_GRAUS = 90;   // EXEMPLO: verificar mecanica e sentido
const int SERVO_FECHADO_GRAUS = 0;   // EXEMPLO
const unsigned int DISTANCIA_DETECCAO_CM = 12; // EXEMPLO
const unsigned int DISTANCIA_LIBERACAO_CM = 20; // EXEMPLO (> deteccao)
const unsigned long TEMPO_ABERTA_MS = 4500; // EXEMPLO: fechamento por tempo
const unsigned long INTERVALO_LEITURA_MS = 150;
const unsigned long INTERVALO_LOG_MS = 1500;
// ==================================================

Servo cancela;
bool postesLigados = false;
bool carroPresente = false;
bool cancelaAberta = false;
unsigned long abertaDesde = 0;
unsigned long ultimaLeitura = 0;
unsigned long ultimoLog = 0;

void escreverPoste(uint8_t pin, bool ligar) {
  digitalWrite(pin, (ligar == LED_ATIVO_EM_HIGH) ? HIGH : LOW);
}

unsigned int medirDistanciaCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  if (us == 0) return 0; // zero = leitura invalida (nao representa distancia zero)
  return (unsigned int)(us / 58UL);
}

void atualizarPostes(int ldr) {
  bool novoEstado = postesLigados;
  if (ESCURO_QUANDO_LEITURA_BAIXA) {
    if (!postesLigados && ldr < LIMIAR_ESCURO - HISTERESE_LDR) novoEstado = true;
    if (postesLigados && ldr > LIMIAR_ESCURO + HISTERESE_LDR) novoEstado = false;
  } else {
    if (!postesLigados && ldr > LIMIAR_ESCURO + HISTERESE_LDR) novoEstado = true;
    if (postesLigados && ldr < LIMIAR_ESCURO - HISTERESE_LDR) novoEstado = false;
  }
  if (novoEstado != postesLigados) {
    postesLigados = novoEstado;
    escreverPoste(PIN_POSTE_1, postesLigados);
    escreverPoste(PIN_POSTE_2, postesLigados);
    Serial.print(F("POSTES: "));
    Serial.println(postesLigados ? F("LIGADOS") : F("DESLIGADOS"));
  }
}

void abrirCancela() {
  cancela.write(SERVO_ABERTO_GRAUS);
  cancelaAberta = true;
  abertaDesde = millis();
  Serial.print(NOME_LADO);
  Serial.println(F("_ABERTA --> CANCELA ABERTA"));
}

void fecharCancela() {
  cancela.write(SERVO_FECHADO_GRAUS);
  cancelaAberta = false;
  Serial.print(NOME_LADO);
  Serial.println(F("_FECHADA --> CANCELA FECHADA (TEMPO ESGOTADO)"));
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_POSTE_1, OUTPUT);
  pinMode(PIN_POSTE_2, OUTPUT);
  escreverPoste(PIN_POSTE_1, false);
  escreverPoste(PIN_POSTE_2, false);
  cancela.attach(PIN_SERVO);
  cancela.write(SERVO_FECHADO_GRAUS);
  Serial.print(F("CONTROLE DO LADO: "));
  Serial.println(NOME_LADO);
}

void loop() {
  const unsigned long agora = millis();
  if (agora - ultimaLeitura >= INTERVALO_LEITURA_MS) {
    ultimaLeitura = agora;
    const int ldr = analogRead(PIN_LDR);
    atualizarPostes(ldr);
    const unsigned int cm = medirDistanciaCm();

    // Histerese para nao disparar varias vezes com o carro parado no sensor.
    if (cm > 0) {
      if (!carroPresente && cm <= DISTANCIA_DETECCAO_CM) {
        carroPresente = true;
        if (!cancelaAberta) abrirCancela();
      } else if (carroPresente && cm >= DISTANCIA_LIBERACAO_CM) {
        carroPresente = false;
      }
    }

    if (agora - ultimoLog >= INTERVALO_LOG_MS) {
      ultimoLog = agora;
      Serial.print(F("Distancia: "));
      if (cm == 0) Serial.print(F("sem leitura"));
      else { Serial.print(cm); Serial.print(F(" cm")); }
      Serial.print(F(" | LDR: "));
      Serial.print(ldr);
      Serial.print(F(" | Postes: "));
      Serial.println(postesLigados ? F("ON") : F("OFF"));
    }
  }

  // Replica proposta de fechamento por tempo. Nao substitui sensor de seguranca.
  if (cancelaAberta && agora - abertaDesde >= TEMPO_ABERTA_MS) {
    fecharCancela();
  }
}
