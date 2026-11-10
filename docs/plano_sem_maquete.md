# Plano de validação sem a maquete

1. **Preservação:** conservar o `backup_arduino_uno.hex` fora do repositório público, junto com sua cópia de segurança. Não anexar dumps do ESP32 com eventuais credenciais.
2. **Documentação:** indicar que o sistema físico original foi desmontado e que a programação apresentada é reconstruída.
3. **Revisão estática:** verificar se os sketches contêm `setup()`/`loop()`, se bibliotecas foram declaradas e se `secrets.h` não está versionado.
4. **Simulação:** rodar os testes Python em `simulacao/test_logica.py`. Eles verificam apenas regras de negócio propostas, **não executam o firmware C++**.
5. **Compilação futura:** usar Arduino IDE com placa Arduino Uno para `controle_lado.ino` e placa ESP32 com biblioteca PubSubClient para `monitor_vagas.ino`.
6. **Protótipo futuro:** testar sensores e LEDs separadamente; confirmar pinagem e níveis elétricos; calibrar LDR e servos; só depois integrar cancelas.
7. **MQTT:** testar com broker privado/de laboratório e configurar MQTT Dash; jamais colocar senha ou token no código público.
8. **Checklist final:** manter status `não testado fisicamente` enquanto não houver nova montagem.

## Comandos de teste da simulação

```bash
python3 -m unittest discover -s simulacao -p 'test_*.py' -v
```

## Testes que continuam pendentes

- Leitura real de sensores das vagas (tipo e polaridade desconhecidos).
- Comunicação Wi-Fi/MQTT real e eventual TLS.
- Acionamento seguro de motores e LEDs.
- Integração das três placas e resposta em MQTT Dash.
