/* ******************** Motor de Passo - Alimentador de Peixes (Versão Simplificada) ********************
   Adaptado por: Matheus Muller, Vinicius Padilha, Germano Lagana
   Rev.: 02 - Motor Isolado
   Data: 28.04.2025
****************************************************************************************** */

// Inclusão da biblioteca do motor de passo
#include <Stepper.h> 

// Define as entradas onde o motor de passo está conectado
#define in1 18
#define in2 19
#define in3 21
#define in4 22

// Número de passos para um acionamento completo
int passosPorAcionamento = 17;
// Número de acionamentos por refeição
int passosRefeicao = 4;

// Define o motor de passo
Stepper mp(passosPorAcionamento, in1, in3, in2, in4);

void setup() {
  // Inicializa a comunicação serial (opcional para depuração)
  Serial.begin(115200);
  Serial.println("Sistema iniciado. O motor será acionado a cada 5 segundos.");

  // Define a velocidade do motor
  mp.setSpeed(500);
}

void loop() {
  alimentarPeixe();  // Aciona o motor
  delay(5000);       // Aguarda 5 segundos antes do próximo acionamento
}

// Função que alimenta os peixes (aciona o motor)
void alimentarPeixe() {
  for (int i = 0; i < passosRefeicao; i++) {
    mp.step(passosPorAcionamento);
  }
  // Desliga bobinas do motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("Motor acionado.");
}