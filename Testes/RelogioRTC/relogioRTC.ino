/*
  TESTE – MÓDULO RTC DS3231
  Integrantes: Matheus Muller, Germano Lagana e Vinicius Padilha

  Este teste inicializa o módulo de relógio em tempo real (RTC) DS3231,
  sincroniza o horário (se necessário) e imprime continuamente a data e a hora
  atual no monitor serial.
*/

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inicia comunicação I2C nos pinos padrão do ESP32
  Wire.begin(21, 22);

  // Inicia o RTC
  if (!rtc.begin()) {
    Serial.println("❌ ERRO: RTC não encontrado!");
    while (1);
  }

  // Caso a hora ainda não tenha sido configurada:
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Configura com a hora do computador

  Serial.println("✅ RTC iniciado com sucesso!");
}

void loop() {
  DateTime agora = rtc.now();

  Serial.print("Data: ");
  Serial.print(agora.day());
  Serial.print("/");
  Serial.print(agora.month());
  Serial.print("/");
  Serial.print(agora.year());

  Serial.print(" | Hora: ");
  Serial.print(agora.hour());
  Serial.print(":");
  Serial.print(agora.minute());
  Serial.print(":");
  Serial.println(agora.second());

  delay(1000);
}
