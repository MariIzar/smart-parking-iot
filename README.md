# Estacionamento Autonomo Inteligente com IoT — reconstrucao

**STATUS: a maquete original foi desmontada. A reconstrução NÃO foi validada em hardware.**

Projeto academico de estacionamento com duas placas Arduino Uno, um ESP32, cancelas, iluminacao automatica por LDR e monitoramento de quatro vagas.

> **AVISO:** este repositorio contem **codigo-fonte reconstruido como proposta**, e NAO uma descompilacao fiel do codigo original. Os pinos, tempos, limiares e tipos dos sensores ainda nao foram confirmados. **NAO grave os sketches nas placas sem conferir a fiacao e preservar os backups originais.**

## O que esta confirmado

- Havia **dois Arduino Uno**, e ambos utilizavam o mesmo codigo, segundo a equipe.
- Cada Arduino estava ligado a **dois postes** e **um LDR**; os postes acendiam no escuro.
- Os Arduinos tambem controlavam as cancelas e sensores de aproximacao.
- O **ESP32** cuidava das **quatro vagas**.
- O relatorio descreve sensores nas vagas, LEDs azuis, uso de Wi-Fi/MQTT e aplicativo MQTT Dash.
- Foi realizado backup de um Uno; no firmware compilado aparecem textos como `SAIDA_ABERTA`, `SAIDA_FECHADA` e `LDR:`. Isso e evidencia parcial, NAO o codigo-fonte.

## Hipoteses da reconstrucao (a validar)

- Cada Uno opera **uma cancela + HC-SR04 + dois postes + um LDR**.
- Os sensores de vaga do ESP32 fornecem **sinal digital ativo em LOW** (tipo nao confirmado).
- Os LED dos postes sao comandados em HIGH; limiar do LDR, angulos dos servos e tempos sao exemplos.
- O ESP32 publica situacao das vagas via MQTT, mas o relatorio nao confirma os topicos exatos nem o broker realmente utilizado.

## Arquivos

- `arduino/controle_lado/controle_lado.ino`: sketch comum para cada Uno, com configuracao `NOME_LADO` (ENTRADA ou SAIDA).
- `esp32/monitor_vagas/monitor_vagas.ino`: proposta de leitura digital de quatro sensores e publicacao MQTT.
- `esp32/monitor_vagas/secrets.example.h`: modelo local de configuracao. Copiar para `secrets.h`; **nunca enviar credenciais ao GitHub**.
- `docs/checagem_hardware.md`: checklist de seguranca e tabela de pinos a confirmar.
- `docs/relatorio_original.pdf`: relatorio fornecido pela equipe.
- `midia/`: foto e videos enviados, para documentar o prototipo.

## Arquitetura proposta

```text
                 Estacionamento
        +-------------+-------------+
        |             |             |
      Uno 1         Uno 2         ESP32
   2 postes/LDR  2 postes/LDR    4 vagas
     cancela      cancela       Wi-Fi/MQTT
```

Os Arduinos podem operar independentemente; **a comunicacao direta Arduino-ESP32 nao esta demonstrada**. O sketch do ESP32 publica somente vagas. Qualquer integracao entre estados das cancelas e MQTT e um desenvolvimento futuro.

## Instalacao e testes

1. **Preservar** os programas originais, nunca sobrescrever placas que funcionam sem backup.
2. Fotografar o circuito e preencher `docs/checagem_hardware.md`.
3. Instalar Arduino IDE e suporte de placa ESP32 (se necessario).
4. Para Uno, abrir `arduino/controle_lado/controle_lado.ino`; conferir todos os pinos e parametros do inicio do arquivo; testar em bancada.
5. Para ESP32, instalar `PubSubClient` na Arduino IDE, copiar `secrets.example.h` para `secrets.h`, preencher configuracao e confirmar tipo/pinos dos sensores.
6. O exemplo MQTT usa TCP sem TLS (porta 1883) **somente para rede local de teste confiavel**. Para HiveMQ Cloud ou outro broker com TLS, adaptar para `WiFiClientSecure` com validacao do certificado antes da implantacao. Nao usar `setInsecure()` como solucao permanente.
7. Conferir leituras pelo Monitor Serial, ajustar limiares e so entao testar acionamentos fisicos.

### Topicos MQTT propostos (nao recuperados)

| Topico | Valor |
|---|---|
| `estacionamento/vagas/1` ate `/4` | `ocupada` / `livre` |
| `estacionamento/vagas_livres` | numero de 0 a 4 |
| `estacionamento/lotado` | `sim` / `nao` |

As mensagens sao publicadas com `retain=true`, para consulta pelo MQTT Dash. O sistema nao implementa autenticacao de usuario nem garantia de entrega em caso de queda da rede.

## Publicar no GitHub

No Terminal, dentro da pasta do projeto (apos extrair o ZIP):

```bash
git init
git add .
git commit -m "Reconstrucao inicial do estacionamento inteligente"
git branch -M main
# Crie no GitHub um repositorio VAZIO, sem README adicional.
git remote add origin https://github.com/SEU_USUARIO/estacionamento-inteligente.git
git push -u origin main
```

Verifique primeiro `git status` e `git ls-files` para confirmar que nao entram segredos ou backups. Pode ser necessario configurar nome/e-mail Git e autenticar sua conta no GitHub.

## Estado atual

- [x] Arquitetura principal documentada a partir do relato e do relatorio
- [x] Propostas iniciais de sketches Arduino e ESP32
- [ ] Pinos e esquemas eletricos confirmados
- [ ] Sketches compilados em Arduino IDE com as bibliotecas/placas corretas
- [ ] Testes praticos concluídos
- [ ] Comunicacao MQTT validada no broker real

**Licenciamento e autores:** preencher com nomes e autorizacoes da equipe antes da publicacao. Nao atribuir aos integrantes codigo que nao escreveram: indicar que e uma reconstrucao assistida a partir do sistema original.

## Material da disciplina e simulacao

- `docs/analise_material_professor.md`: comparacao com o ZIP do professor e itens ainda nao demonstrados.
- `docs/plano_sem_maquete.md`: plano para validacao sem montagem fisica.
- `simulacao/`: modelo Python independente do firmware para testar regras propostas de LDR, vagas e deteccao.

Os arquivos do professor **nao estao redistribuidos** aqui.
