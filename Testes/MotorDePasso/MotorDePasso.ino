/*
  TESTE DO MOTOR DE PASSO 28BYJ-48 COM ESP32
  Grupo: Matheus Muller, Germano Lagana, Vinicius Padilha
  Objetivo: Verificar o funcionamento do motor de passo utilizando as portas digitais do ESP32.

  OBS: Certifique-se de que os fios do driver ULN2003 estão corretamente conectados às portas digitais indicadas.
*/

#include <Stepper.h>

// Define o número de passos por volta do motor (para o 28BYJ-48 com redutor: 2048 passos)
#define PASSOS_POR_REVOLUCAO 2048

// Define os pinos conectados ao driver do motor (ULN2003)
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14

// Cria o objeto do motor de passo com os pinos definidos
Stepper motor(PASSOS_POR_REVOLUCAO, IN1, IN3, IN2, IN4);

void setup() {
  // Inicia a comunicação serial para feedback no monitor
  Serial.begin(115200);
  Serial.println("Iniciando teste do motor de passo...");

  // Define a velocidade do motor (em rotações por minuto)
  motor.setSpeed(10);
}

void loop() {
  Serial.println("Girando no sentido horário 1/2 volta...");
  motor.step(PASSOS_POR_REVOLUCAO / 2); // Gira 1024 passos (meia volta)
  delay(2000);

  Serial.println("Girando no sentido anti-horário 1/2 volta...");
  motor.step(-PASSOS_POR_REVOLUCAO / 2); // Gira -1024 passos (meia volta)
  delay(2000);
}
