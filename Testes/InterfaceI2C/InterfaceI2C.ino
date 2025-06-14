/*
  Teste da Interface I2C – Scanner de dispositivos
  Integrantes: Matheus Muller, Germano Lagana e Vinicius Padilha
  Objetivo: Verificar se o barramento I2C está funcionando corretamente e quais dispositivos estão conectados.
*/

#include <Wire.h>

void setup() {
  Serial.begin(115200);           // Inicia comunicação serial
  Wire.begin(21, 22);             // Pinos padrão do ESP32: SDA = 21, SCL = 22
  delay(1000);
  Serial.println("🔍 Iniciando scanner I2C...");
}

void loop() {
  byte count = 0;

  Serial.println("📡 Varredura dos dispositivos I2C...");
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("✅ Dispositivo encontrado no endereço 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();
      count++;
      delay(10);
    }
  }

  if (count == 0) {
    Serial.println("⚠️ Nenhum dispositivo I2C encontrado.");
  } else {
    Serial.print("🔢 Total de dispositivos encontrados: ");
    Serial.println(count);
  }

  Serial.println("🕒 Aguardando 5 segundos para nova varredura...\n");
  delay(5000);  // Aguarda antes de repetir
}
