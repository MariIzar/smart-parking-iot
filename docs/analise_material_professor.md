# Cruzamento com o material da disciplina IoT

**Origem:** ZIP do professor `course_files_export.zip`, consultado para orientar esta reconstrução. Os materiais didáticos do professor **não foram incluídos neste repositório**, para evitar republicação sem autorização.

## O que o material apoia

| Material da disciplina | Aplicação possível no estacionamento | Estado |
|---|---|---|
| `Código para ESP32/template_IoT.ino` | Wi-Fi, `PubSubClient`, publicação e assinatura MQTT | Exemplo de referência; código da equipe não recuperado |
| `Código para Sensor Ultrassônico/Ultrassom.txt` | Leitura HC-SR04 de aproximação das cancelas | Base didática; pinagem original desconhecida |
| `Códigos/Função millis.docx` | Temporização não bloqueante das cancelas | Técnica aplicada na proposta Arduino |
| `Apostila_Trabalhos_IoT_V2.docx` | Sensores, atuadores, nuvem, app, etapas do projeto | Requisitos da disciplina |
| `Cap_4_MQTT.pptx` e `Cap_5_MQTT_DASH.ppt` | Tópicos e visualização mobile | Referência conceitual |
| `Tutoriais_Instalação_Python_BD...docx` e `Código para BD/bd.py` | Registro MQTT em MySQL | Extensão **não implementada** |
| `UTILIZAÇÃO DO ESP32 EM PROJETOS_v1.docx` | Cuidado com alimentação e GPIO de 3,3 V | Atenção para montagem futura |

## Diferenças / não comprovado

- A apostila de trabalhos práticos prevê **comandos de atuação pelo aplicativo**, além do envio dos dados. Os arquivos reconstruídos publicam estados das vagas; não comprovam comando remoto para cancelas/LEDs.
- A apostila inclui etapas de banco de dados; **não há evidência suficiente** de que a maquete utilizou MySQL no projeto final.
- O template ESP32 fornecido pelo professor demonstra publicação de temperatura **simulada**, não é o código recuperado das vagas.
- O relatório do estacionamento menciona MQTT Dash, mas **não comprova** broker, tópicos, credenciais ou estrutura de mensagens efetivamente utilizados.
- **A maquete foi desmontada**, portanto não há validação elétrica nem teste atual em hardware.

## Evidência, hipótese e proposta

- **Relato da equipe:** dois Arduinos Uno com o mesmo sketch; cada um tinha um LDR e dois postes, que acendiam no escuro; os Arduinos controlavam cancelas; ESP32 cuidava das quatro vagas.
- **Arquivo original preservado separadamente:** backup de um Uno em `.hex`; trata-se de firmware compilado e não prova integral de pinagem ou código-fonte.
- **Hipótese da reconstrução:** cada Uno controla uma cancela + ultrassom + dois postes; ESP32 recebe quatro sensores digitais e publica por MQTT.
- **Ainda faltam:** modelo dos quatro sensores de vaga, pinos, limiar do LDR, ângulos de servo, lógica de ocupação e testes em bancada.

## Segurança e autoria

Os exemplos da disciplina contêm credenciais Wi-Fi embutidas. Não reproduzir esses dados no GitHub; caso pertençam à equipe ou estejam em uso, alterá-los. Os sketches deste repositório são **novas implementações assistidas**, não os arquivos originais do semestre. Não declarar que foram testados na maquete desmontada.
