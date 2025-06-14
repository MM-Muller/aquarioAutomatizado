/*
  TESTE – DISPLAY LCD 16x2 I2C
  Integrantes: Matheus Muller, Germano Lagana e Vinicius Padilha

  Este teste inicializa o display LCD 16x2 com interface I2C e exibe duas linhas
  de texto fixo. Deve-se ver a mensagem "Aquário Inteligente" na primeira linha
  e "Display Funcionando" na segunda.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======= CONFIGURAÇÃO DO DISPLAY =======
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C pode ser 0x27 ou 0x3F. Verifique se necessário!

void setup() {
  // Inicializa a comunicação I2C com ESP32 nos pinos padrão (21=SDA, 22=SCL)
  Wire.begin(21, 22);

  // Inicia o display e acende a luz de fundo
  lcd.init();
  lcd.backlight();

  // Limpa o display
  lcd.clear();

  // Define o que será exibido
  lcd.setCursor(0, 0); // Coluna 0, linha 0
  lcd.print("Aquario Inteligente");

  lcd.setCursor(0, 1); // Coluna 0, linha 1
  lcd.print("Display funcionando");

  // Mensagem no serial para confirmação
  Serial.begin(115200);
  Serial.println("Display LCD iniciado com sucesso!");
}

void loop() {
  // Nada a repetir neste teste
}
