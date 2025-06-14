/*
  TESTE – SENSOR DE TEMPERATURA DS18B20
  Integrantes: Matheus Muller, Germano Lagana e Vinicius Padilha

  Este teste lê a temperatura da água do aquário utilizando o sensor DS18B20.
  A leitura é feita via protocolo OneWire e exibida no Monitor Serial.
*/

#include <OneWire.h>
#include <DallasTemperature.h>

// ======= CONFIGURAÇÃO DO PINO DO SENSOR =======
#define ONE_WIRE_BUS 32  // Pino digital onde o sensor está conectado (ajuste se necessário)

OneWire oneWire(ONE_WIRE_BUS);           // Inicializa comunicação OneWire
DallasTemperature sensors(&oneWire);     // Usa biblioteca Dallas para ler o DS18B20

void setup() {
  Serial.begin(115200);                  // Inicia comunicação serial
  sensors.begin();                       // Inicia o sensor
  Serial.println("Teste do Sensor de Temperatura iniciado!");
}

void loop() {
  sensors.requestTemperatures();         // Solicita leitura ao sensor
  float temperatura = sensors.getTempCByIndex(0);  // Lê a temperatura em Celsius

  // Verifica se a leitura é válida
  if (temperatura == DEVICE_DISCONNECTED_C) {
    Serial.println("Erro: Sensor desconectado ou leitura inválida!");
  } else {
    Serial.print("Temperatura atual: ");
    Serial.print(temperatura);
    Serial.println(" °C");
  }

  delay(2000);  // Aguarda 2 segundos antes da próxima leitura
}
