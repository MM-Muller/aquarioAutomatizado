#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <RTClib.h>

// WiFi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA_WIFI";

// Telegram
#define BOT_TOKEN "SEU_BOT_TOKEN"  // Bot token fornecido pelo @BotFather
#define CHAT_ID "SEU_CHAT_ID"       // Chat ID para onde enviar a mensagem

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// RTC
RTC_DS3231 rtc;

// Simulação de dados de temperatura e luz
float temperaturaAtual = 25.3; // Valor exemplo
bool luzLigada = false; // Estado da luz

void setup() {
  Serial.begin(115200);
  
  // Conecta no WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando no WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
  
  // Permite HTTPS (necessário para o Telegram)
  client.setInsecure();
  
  // Inicializa o RTC
  if (!rtc.begin()) {
    Serial.println("Erro ao inicializar RTC!");
    while (1);
  }
  
  // (opcional) ajustar o RTC caso necessário:
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // Aguarda conexão
  delay(1000);
}

void loop() {
  // Lê o horário atual
  DateTime now = rtc.now();
  
  // (opcional) Simular leitura real da temperatura
  temperaturaAtual += random(-5, 5) * 0.1;

  // Formatar mensagem
  String mensagem = "📋 Status do Sistema\n";
  mensagem += "🕒 Hora atual: ";
  mensagem += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "\n";
  mensagem += "🌡️ Temperatura: ";
  mensagem += String(temperaturaAtual, 1) + "°C\n";
  mensagem += "💡 Luz: ";
  mensagem += luzLigada ? "Ligada" : "Desligada";

  // Envia para o Telegram
  bot.sendMessage(CHAT_ID, mensagem, "");
  
  Serial.println("Mensagem enviada!");

  // Aguarda 1 hora para enviar de novo (ou ajuste como quiser)
  delay(3600000); 
}
