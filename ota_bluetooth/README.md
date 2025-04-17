# ESP32 Bluetooth OTA

Atualize o firmware do seu ESP32 via **Bluetooth Serial (SPP)** utilizando ferramentas como o **CoolTerm**. Esse projeto permite que você envie um arquivo `.bin` diretamente para o ESP32, facilitando o processo de atualização sem precisar de cabos USB.

---

## Funcionalidades

- Conexão Bluetooth (Serial SPP)
- Recebimento do firmware `.bin` via Bluetooth
- Escrita do novo firmware usando a biblioteca `Update.h`
- Reinicialização automática ao fim da atualização
- Timeout de 10 segundos para finalização segura da OTA

---

## Como usar

### 1. Pré-requisitos

- ESP32 (com Bluetooth)
- Arduino IDE configurada para ESP32
- Ferramenta para enviar arquivos via serial Bluetooth (ex: **CoolTerm**)

### 2. Upload do Firmware OTA (este código)

1. Abra o Arduino IDE
2. Faça upload do sketch `ota_buetooth.ino` para o ESP32 via USB
3. O ESP32 aparecerá como um dispositivo Bluetooth com o nome:  
   **`ESP32_OTA_BT`**

---

## Enviando o novo firmware via CoolTerm

1. **Conecte ao ESP32 via Bluetooth Serial**  
   No CoolTerm, configure a porta COM correta e conecte-se ao dispositivo Bluetooth.

2. **Envie o arquivo `.bin`**
   - Vá em **Connection > Send Text File**
   - Selecione o arquivo `.bin` gerado pela compilação do novo firmware
   - Aguarde a transferência (o ESP32 imprimirá os bytes recebidos via Serial Monitor)

3. **Aguarde a finalização**
   - Se todos os dados forem recebidos corretamente, o ESP32 reiniciará automaticamente com o novo firmware.
   - Se houver timeout ou erro, o ESP32 tentará reiniciar ou informar o erro pela serial.

---

## Exemplo de uso (Serial Monitor)

```text
ESP32 OTA via Bluetooth iniciado!
Iniciando OTA via Bluetooth...
Atualização iniciada...
Bytes recebidos: 64
Bytes recebidos: 128
...
OTA concluída com sucesso!
Reiniciando...
```
## Como funciona

O código escuta dados pela SerialBT. Assim que os dados começam a ser recebidos, o ESP32 inicia o processo de atualização usando a biblioteca Update.h. Ele grava os blocos de dados de 64 bytes no firmware interno. Ao final, reinicia automaticamente.

Se um bloco for menor que 64 bytes ou se passar mais de 10 segundos sem receber dados, ele tenta finalizar o OTA.

## Desenvolvido por:

**Laiza Gordiano** 