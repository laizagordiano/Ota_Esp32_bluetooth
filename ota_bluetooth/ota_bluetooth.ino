#include <BluetoothSerial.h>
#include <Update.h>

BluetoothSerial SerialBT;

// Temporizador OTA
unsigned long timer1Start = 0;
unsigned long timer1Interval = 10000;
bool timer1Ativo = false;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_OTA_BT");
  Serial.println("ESP32 OTA via Bluetooth iniciado!");
}

void loop() {
  if (SerialBT.available()) {
    handleOTA();
  }

  delay(10);  // Pequeno atraso para evitar travamentos

  // Timeout da OTA
  if (timer1Ativo && millis() - timer1Start >= timer1Interval) {
    Serial.println("Timeout atingido. Tentando finalizar OTA...");
    if (Update.end(true)) {
      Serial.println("OTA concluída com sucesso!");
      SerialBT.println("Ota com Sucesso");
      ESP.restart();
    } else {
      Serial.println("Erro ao finalizar OTA!");
      Serial.printf("Erro: %s\n", Update.errorString());
      SerialBT.println("Erro ao finalizar OTA!");
      delay(2000);
      ESP.restart();
    }
  }
}

void handleOTA() {
  static size_t totalBytes = 0;
  static unsigned long lastReceiveTime = millis();
  const unsigned long timeout = 10000;

  if (!Update.isRunning()) {
    Serial.println("Iniciando OTA via Bluetooth...");
    if (Update.begin(UPDATE_SIZE_UNKNOWN)) {
      Serial.println("Atualização iniciada...");
    } else {
      Serial.println("Falha ao iniciar atualização!");
      return;
    }
  }

  while (SerialBT.available()) {
    uint8_t buffer[64];
    size_t len = SerialBT.readBytes(buffer, sizeof(buffer));
    if (Update.write(buffer, len)) {
      totalBytes += len;
      lastReceiveTime = millis();
      Serial.printf("Bytes recebidos: %d\n", totalBytes);

      if (len != 64) {
        Serial.println("Falta de bytes no recebimento");
        SerialBT.printf("Menos de 64 bytes recebidos. Total: %d\n", totalBytes);

        if (Update.end(true)) {
          Serial.println("OTA concluída com sucesso!");
          SerialBT.println("Ota com Sucesso");
        } else {
          Serial.println("Erro ao finalizar OTA!");
          Serial.printf("Erro: %s\n", Update.errorString());
          SerialBT.println("Erro ao finalizar OTA!");
        }

        delay(2000);
        ESP.restart();
      }

      // Reinicia temporizador
      timer1Start = millis();
      timer1Ativo = true;
    }
  }

  if (millis() - lastReceiveTime > timeout) {
    Serial.println("Timeout atingido. Tentando finalizar OTA...");
    if (Update.end(true)) {
      Serial.println("OTA concluída com sucesso!");
      ESP.restart();
    } else {
      Serial.println("Erro ao finalizar OTA!");
      Serial.printf("Erro: %s\n", Update.errorString());
      SerialBT.println("Erro ao finalizar OTA!");
      delay(2000);
      ESP.restart();
    }
  } else {
    Serial.printf("Aguardando dados... Tempo desde o último: %lu ms\n", millis() - lastReceiveTime);
  }
}
