/* ******************** Alimentador para peixes do Manual do Mundo (Adaptado ESP32) ********************
   Adaptado por: Matheus Muller, Vinicius Padilha, Germano Lagana
   Rev.: 02
   Data: 28.04.2025
***************************************************************************** */

// Inclusão das bibliotecas
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <Stepper.h> 
#include <RTClib.h> 

RTC_DS3231 rtc;

// Define o endereço do LCD e o tamanho da tela
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define as entradas onde o motor de passo está conectado
#define in1 13
#define in2 12
#define in3 14
#define in4 15

int passosPorAcionamento = 32;

int passosRefeicao = 4;

// Define o motor de passo
Stepper mp(passosPorAcionamento, in1, in3, in2, in4);

// Parâmetros de horário que serão atualizados
int horaAtual, minutoAtual;
// Parâmetros horários de alimentação
int demosComida1, demosComida2;

/* ******************** CONFIGURAÇÃO DE HORÁRIO PARA ALIMENTAÇÃO ********************/

// Primeira alimentação
#define horaAlimentacao1 20
#define minutoAlimentacao1 30

// Segunda alimentação
#define horaAlimentacao2 8
#define minutoAlimentacao2 30

/***************************************************************************** */

void setup() {
  // Inicializa a comunicação serial (opcional para depuração)
  Serial.begin(115200);

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();

  // Inicializa o motor
  mp.setSpeed(500);

  // Inicializa o RTC
  if (!rtc.begin()) {
    Serial.println("Não foi possível encontrar o módulo RTC");
    while (1);
  }

  // Se o RTC perdeu a alimentação, ajustar o horário:
  if (rtc.lostPower()) {
    Serial.println("RTC perdeu a alimentação, ajustando hora...");
    // Define o horário inicial (ano, mês, dia, hora, minuto, segundo)
    rtc.adjust(DateTime(2025, 4, 28, 12, 0, 0));
  }

  demosComida1 = 0;
  demosComida2 = 0;
}

void loop() {
  // Obtém a hora atual
  DateTime now = rtc.now();
  horaAtual = now.hour();
  minutoAtual = now.minute();

  // Verifica se é o horário da primeira alimentação
  if (horaAtual == horaAlimentacao1 && minutoAtual == minutoAlimentacao1 && demosComida1 == 0) {
    alimentarPeixe();
    demosComida1 = 1;
  }

  // Verifica se é o horário da segunda alimentação
  if (horaAtual == horaAlimentacao2 && minutoAtual == minutoAlimentacao2 && demosComida2 == 0) {
    alimentarPeixe();
    demosComida2 = 1;
  }

  // Atualiza LCD
  if (demosComida1 == 0 || (demosComida1 == 1 && demosComida2 == 1)) {
    horarioNaTela(now);
    alimentacao1Tela();
  }

  if (demosComida1 == 1 && demosComida2 == 0) {
    horarioNaTela(now);
    alimentacao2Tela();
  }

  // Reseta alimentação à meia-noite
  if (horaAtual == 0 && minutoAtual == 0) {
    demosComida1 = 0;
    demosComida2 = 0;
  }

  delay(1000);
}

// Função que alimenta os peixes
void alimentarPeixe() {
  for (int i = 0; i < passosRefeicao; i++) {
    mp.step(passosPorAcionamento);
  }
  // Desliga bobinas do motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000);
}

// Função que exibe o horário atual no LCD
void horarioNaTela(DateTime now) {
  char horaRelogioStr[17];
  sprintf(horaRelogioStr, "Hora: %02d:%02d:%02d", now.hour(), now.minute(), now.second());
  lcd.setCursor(0, 0);
  lcd.print(horaRelogioStr);
}

// Função que exibe o próximo horário de alimentação 1
void alimentacao1Tela() {
  char horaMinutoStr[17];
  sprintf(horaMinutoStr, "Prox: %02d:%02d:00", horaAlimentacao1, minutoAlimentacao1);
  lcd.setCursor(0, 1);
  lcd.print(horaMinutoStr);
}

// Função que exibe o próximo horário de alimentação 2
void alimentacao2Tela() {
  char horaMinutoStr[17];
  sprintf(horaMinutoStr, "Prox: %02d:%02d:00", horaAlimentacao2, minutoAlimentacao2);
  lcd.setCursor(0, 1);
  lcd.print(horaMinutoStr);
}
