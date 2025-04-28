#include <Wire.h>
#include <RTClib.h>

#define RELAY_PIN 16  // GPIO para controlar a luz (relé)

// Inicializar o RTC
RTC_DS3231 rtc;  // Relógio de tempo real

// Variáveis de horário
int horaAtual, minutoAtual;
int horaLigamentoLuz = 18, minutoLigamentoLuz = 0; // 18:00 
int horaDesligamentoLuz = 6, minutoDesligamentoLuz = 0; // 06:00 

void setup() {
  // Inicializar o LCD e RTC
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Luz apagada inicialmente
  
  // Verifica se o RTC está funcionando
  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o RTC");
    while (1);
  }
  
  // Ajuste de tempo (faça isso manualmente se necessário)
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  // Obter hora atual
  DateTime now = rtc.now();
  horaAtual = now.hour();
  minutoAtual = now.minute();
  
  // Verificar se é hora de ligar a luz
  if (horaAtual == horaLigamentoLuz && minutoAtual == minutoLigamentoLuz) {
    digitalWrite(RELAY_PIN, HIGH);  // Liga a luz
  }
  
  // Verificar se é hora de desligar a luz
  if (horaAtual == horaDesligamentoLuz && minutoAtual == minutoDesligamentoLuz) {
    digitalWrite(RELAY_PIN, LOW);  // Desliga a luz
  }

  // Atraso de 1 segundo para evitar leituras rápidas demais
  delay(1000);
}
