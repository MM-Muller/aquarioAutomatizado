#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pino para o sensor DS18B20 (GPIO15)
#define ONE_WIRE_BUS 15 

// Inicializa a comunicação OneWire e o sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Inicializa o LCD (endereço I2C 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2); // (endereço, 16 colunas, 2 linhas)

void setup() {
  // Inicia a comunicação serial
  Serial.begin(115200);

  // Inicia o LCD
  lcd.init();
  lcd.backlight();

  // Inicia o sensor DS18B20
  sensors.begin();
}

void loop() {
  // Solicita a leitura da temperatura do sensor
  sensors.requestTemperatures();
  
  // Obtém a temperatura do primeiro sensor (DS18B20)
  float temperaturaC = sensors.getTempCByIndex(0);

  // Verifica se a leitura foi bem-sucedida
  if (temperaturaC != DEVICE_DISCONNECTED_C) {
    // Exibe a temperatura no LCD
    lcd.clear();  // Limpa o LCD
    lcd.setCursor(0, 0);
    lcd.print("Temperatura:");
    lcd.setCursor(0, 1);
    lcd.print(temperaturaC);
    lcd.print(" C");
  } else {
    // Se o sensor não estiver conectado corretamente
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro no sensor");
  }

  // Exibe a temperatura também no monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperaturaC);
  Serial.println(" C");

  // Atraso de 1 segundo antes de nova leitura
  delay(1000);
}