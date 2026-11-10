# Checklist de verificacao ANTES de gravar

- [ ] Registrar foto legivel de cada Arduino e do ESP32 e seus pinos.
- [ ] Confirmar se existem dois Arduinos Uno iguais.
- [ ] Identificar a funcao de cada placa (entrada/saida) e comparar os firmwares, se possivel.
- [ ] Confirmar as conexoes dos dois postes e de um LDR por Arduino.
- [ ] Identificar o divisor resistivo do LDR e calibrar qual lado do limiar representa escuro.
- [ ] Conferir os pinos de TRIG/ECHO dos sensores de entrada e saida.
- [ ] Conferir alimentacao dos servos por fonte adequada e GND comum; nao alimentar servos de carga pela USB.
- [ ] Verificar o sentido de giro/limites mecanicos dos servos sem haste acoplada.
- [ ] Verificar limite de corrente dos LEDs/resistores e eventual transistor/driver.
- [ ] Identificar o modelo dos quatro sensores de vagas (digital, analogico, IR, ultrassonico etc.).
- [ ] Confirmar que nenhum sinal de 5 V vai diretamente a GPIO do ESP32 (3,3 V).
- [ ] Conferir se sensores digitais sao ativos em HIGH ou LOW.
- [ ] Guardar backup original fora do GitHub.
- [ ] Testar leitura dos sensores isoladamente antes de acionar cancelas.
- [ ] Testar em bancada com cancelas desacopladas e obstaculos ausentes.
- [ ] Validar protecao contra fechamento sobre objetos; fechamento por tempo do exemplo NAO e suficiente para instalacao real.
- [ ] Identificar broker MQTT e exigencia de TLS/certificado.

## Tabela de pinos (preencher apos fotografia)

| Placa | Funcao | Pino real | Pino do codigo (EXEMPLO) |
|---|---|---|---|
| Uno 1/2 | TRIG | pendente | D7 |
| Uno 1/2 | ECHO | pendente | D8 |
| Uno 1/2 | Servo | pendente | D9 |
| Uno 1/2 | Poste 1 | pendente | D4 |
| Uno 1/2 | Poste 2 | pendente | D5 |
| Uno 1/2 | LDR | pendente | A0 |
| ESP32 | Vaga 1 | pendente | GPIO16 |
| ESP32 | Vaga 2 | pendente | GPIO17 |
| ESP32 | Vaga 3 | pendente | GPIO18 |
| ESP32 | Vaga 4 | pendente | GPIO19 |
