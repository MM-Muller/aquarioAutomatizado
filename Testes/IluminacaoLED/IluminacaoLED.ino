/*
  TESTE DA FITA DE LED WS2812B COM ESP32
  Grupo: Matheus Muller, Germano Lagana, Vinicius Padilha
  Objetivo: Verificar o funcionamento da fita de LED (WS2812B) com controle de cor e brilho via ESP32.

  OBS: Certifique-se de que:
  - O pino de dados (Data IN) da fita está conectado ao pino 5 do ESP32;
  - A alimentação está adequada (idealmente com fonte externa 5V se tiver muitos LEDs);
  - GND do ESP32 e da fonte externa estão interligados.
*/

#include <FastLED.h>

// Define o pino de dados e o número de LEDs
#define DATA_PIN 5
#define NUM_LEDS 30

// Cria o array que representa os LEDs
CRGB leds[NUM_LEDS];

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);
  Serial.println("Iniciando teste da fita de LED...");

  // Inicializa os LEDs (modelo WS2812B com ordem de cores GRB)
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100); // Define brilho (0 a 255)
}

void loop() {
  Serial.println("Cor: Vermelho");
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(1000);

  Serial.println("Cor: Verde");
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(1000);

  Serial.println("Cor: Azul");
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(1000);

  Serial.println("Desligando LEDs...");
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000);
}
