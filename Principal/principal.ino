// ============================
// PROJETO: Aquário Inteligente com ESP32
// Integrantes: Matheus Muller, Germano Lagana e Vinicius Padilha
// ============================

// Bibliotecas necessárias para cada componente
#include <WiFi.h>                   // Comunicação Wi-Fi
#include <WebServer.h>             // Criação de servidor web
#include <Wire.h>                  // Comunicação I2C
#include <RTClib.h>                // Relógio de tempo real (RTC)
#include <LiquidCrystal_I2C.h>     // Display LCD via I2C
#include <OneWire.h>               // Comunicação OneWire
#include <DallasTemperature.h>     // Sensor de temperatura DS18B20
#include <FastLED.h>               // Controle da fita de LED WS2812B
#include <Stepper.h>               // Controle do motor de passo

// ============================
// CONFIGURAÇÃO DE REDE Wi-Fi (modo AP)
// ============================
const char* ssid = "AQUARIO";
const char* senha = "aquario123";
WebServer server(80);  // Cria um servidor na porta 80

// ============================
// INSTÂNCIAS DOS COMPONENTES
// ============================
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Endereço I2C 0x27, display 16x2
RTC_DS3231 rtc;                       // Relógio de tempo real DS3231

// Motor de passo 28BYJ-48 (sequência de pinos)
#define in1 25
#define in2 26
#define in3 27
#define in4 14
Stepper motor(2048, in1, in3, in2, in4);  // 2048 passos por volta

// Sensor de temperatura DS18B20
#define ONE_WIRE_BUS 32
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Fita de LED WS2812B
#define DATA_PIN 5
#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

// ============================
// VARIÁVEIS GLOBAIS
// ============================
String peixeSelecionado = "betta";
float temperaturaAtual = 0.0;
bool motorAtivadoHoje = false;
String modoIluminacao = "com_luz";

int alimentacoesDia = 2;
int horarios[3][2] = {{8, 0}, {20, 0}, {23, 0}};  // Horários de alimentação

unsigned long tempoAnterior = 0;
int telaAtual = 0;  // Tela atual exibida no LCD

// ============================
// CONFIGURAÇÃO DO PEIXE ESCOLHIDO
// ============================
void configurarPeixe(String peixe) {
  peixeSelecionado = peixe;

  // Configura cor da fita de LED e número de alimentações
  if (peixe == "betta") {
    fill_solid(leds, NUM_LEDS, CRGB::LightBlue);
    alimentacoesDia = 2;
  } else if (peixe == "neon") {
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    alimentacoesDia = 2;
  } else if (peixe == "oscar") {
    fill_solid(leds, NUM_LEDS, CRGB::Orange);
    alimentacoesDia = 3;
  }
  FastLED.show();  // Atualiza a cor dos LEDs
}

// ============================
// VERIFICA SE A TEMPERATURA ESTÁ IDEAL
// ============================
bool temperaturaIdeal() {
  if (peixeSelecionado == "betta") return temperaturaAtual >= 26 && temperaturaAtual <= 28;
  if (peixeSelecionado == "neon")  return temperaturaAtual >= 22 && temperaturaAtual <= 26;
  if (peixeSelecionado == "oscar") return temperaturaAtual >= 25 && temperaturaAtual <= 27;
  return true;
}

// ============================
// RETORNA STATUS DA TEMPERATURA
// ============================
String statusTemperatura() {
  if (peixeSelecionado == "betta") {
    if (temperaturaAtual < 26) return "Baixa";
    if (temperaturaAtual > 28) return "Alta";
  } else if (peixeSelecionado == "neon") {
    if (temperaturaAtual < 22) return "Baixa";
    if (temperaturaAtual > 26) return "Alta";
  } else if (peixeSelecionado == "oscar") {
    if (temperaturaAtual < 25) return "Baixa";
    if (temperaturaAtual > 27) return "Alta";
  }
  return "OK";
}

// ============================
// CALCULA PRÓXIMA REFEIÇÃO COM BASE NA HORA ATUAL
// ============================
String proximaRefeicao(DateTime agora) {
  for (int i = 0; i < alimentacoesDia; i++) {
    if (agora.hour() < horarios[i][0] || (agora.hour() == horarios[i][0] && agora.minute() < horarios[i][1])) {
      char buffer[6];
      sprintf(buffer, "%02d:%02d", horarios[i][0], horarios[i][1]);
      return String(buffer);
    }
  }
  // Se já passou todas, retorna o primeiro horário do próximo dia
  char buffer[6];
  sprintf(buffer, "%02d:%02d", horarios[0][0], horarios[0][1]);
  return String(buffer);
}

// ============================
// ATUALIZA O DISPLAY LCD DE ACORDO COM A TELA
// ============================
void atualizarLCD(DateTime agora) {
  char linha1[17], linha2[17];
  lcd.clear();

  if (telaAtual == 0) {
    sprintf(linha1, "Hora: %02d:%02d", agora.hour(), agora.minute());
    sprintf(linha2, "Prox: %s", proximaRefeicao(agora).c_str());
  } else if (telaAtual == 1) {
    sprintf(linha1, "Temp: %.1fC", temperaturaAtual);
    sprintf(linha2, "Status: %s", statusTemperatura().c_str());
  } else {
    sprintf(linha1, "Peixe: %s", peixeSelecionado.c_str());
    if (peixeSelecionado == "betta") sprintf(linha2, "Luz: Azul Claro");
    if (peixeSelecionado == "neon")  sprintf(linha2, "Luz: Verde");
    if (peixeSelecionado == "oscar") sprintf(linha2, "Luz: Laranja");
  }

  lcd.setCursor(0, 0); lcd.print(linha1);
  lcd.setCursor(0, 1); lcd.print(linha2);
}

// ============================
// CONSTRÓI A PÁGINA WEB HTML
// ============================
void paginaWeb() {
  String html = R"rawliteral(
    <!DOCTYPE html><html><head><meta charset='utf-8'><title>Aquário Inteligente</title><style>
    body {font-family: Arial; background: #eef; max-width: 700px; margin: auto; padding: 20px;}
    h2 {color: #003366;}
    .card {background: white; padding: 10px; border-radius: 10px; margin-top: 10px; box-shadow: 0 0 6px #999;}
    </style></head><body>
    <h2>Preferências de cada peixe</h2>
    <div class='card'><b>🐠 Betta</b><br>Luz: Azul Claro<br>Temperatura: 26–28°C<br>Alimentação: 2x/dia<br><i>Betta prefere luz suave, simulando águas calmas e tropicais.</i></div>
    <div class='card'><b>🐟 Neon Tetra</b><br>Luz: Verde<br>Temperatura: 22–26°C<br>Alimentação: 2x/dia<br><i>A luz verde simula vegetação densa, ambiente natural para Neon.</i></div>
    <div class='card'><b>🐡 Oscar</b><br>Luz: Laranja<br>Temperatura: 25–27°C<br>Alimentação: 3x/dia<br><i>Oscar vive bem com luz quente, típica de águas barrentas.</i></div>
    <hr>
    <h2>Configurações</h2>
    <label>Peixe selecionado:</label><br>
    <select id='peixe'>
      <option value='betta'>Betta</option>
      <option value='neon'>Neon Tetra</option>
      <option value='oscar'>Oscar</option>
    </select><br><br>
    <label>Ambiente do aquário:</label><br>
    <select id='ambiente'>
      <option value='com_luz'>Recebe luz natural (ex: janela)</option>
      <option value='sem_luz'>Ambiente fechado (ex: quarto)</option>
    </select><br><br>
    <button onclick='enviar()'>Confirmar</button>
    <p id='resposta'></p>
    <script>
      function enviar() {
        let peixe = document.getElementById('peixe').value;
        let ambiente = document.getElementById('ambiente').value;
        fetch('/config?peixe=' + peixe + '&ambiente=' + ambiente)
          .then(r => r.text()).then(d => document.getElementById('resposta').innerText = d);
      }
    </script>
  </body></html>)rawliteral";

  server.send(200, "text/html", html);
}

// ============================
// SETUP: INICIALIZAÇÃO DOS COMPONENTES
// ============================
void setup() {
  Serial.begin(115200);

  // Cria rede Wi-Fi no modo ponto de acesso (AP)
  WiFi.softAP(ssid, senha);
  Serial.println("Rede criada!");
  Serial.println(WiFi.softAPIP());

  // Roteamento das páginas do servidor
  server.on("/", paginaWeb);
  server.on("/config", HTTP_GET, []() {
    if (server.hasArg("peixe")) configurarPeixe(server.arg("peixe"));
    if (server.hasArg("ambiente")) modoIluminacao = server.arg("ambiente");
    server.send(200, "text/plain", "Configurações atualizadas!\nPeixe: " + peixeSelecionado + "\nAmbiente: " + modoIluminacao);
  });
  server.begin();

  // Inicialização dos módulos
  Wire.begin(21, 22);    // I2C nos pinos padrão do ESP32
  rtc.begin();
  lcd.init(); lcd.backlight();
  sensors.begin();
  motor.setSpeed(10);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  configurarPeixe(peixeSelecionado);
}

// ============================
// LOOP PRINCIPAL
// ============================
void loop() {
  server.handleClient();                  // Lida com requisições da página
  DateTime agora = rtc.now();            // Obtém hora atual do RTC
  sensors.requestTemperatures();         // Solicita leitura de temperatura
  temperaturaAtual = sensors.getTempCByIndex(0);  // Lê temperatura

  // Alterna telas no LCD a cada 5 segundos
  if (millis() - tempoAnterior > 5000) {
    telaAtual = (telaAtual + 1) % 3;
    tempoAnterior = millis();
    atualizarLCD(agora);
  }

  // Verifica se é hora de alimentar o peixe
  for (int i = 0; i < alimentacoesDia; i++) {
    if (agora.hour() == horarios[i][0] && agora.minute() == horarios[i][1] && agora.second() == 0 && !motorAtivadoHoje) {
      motor.step(2048 * 0.25);  // Gira 1/4 de volta
      digitalWrite(in1, LOW); digitalWrite(in2, LOW);
      digitalWrite(in3, LOW); digitalWrite(in4, LOW);
      motorAtivadoHoje = true;
    }
  }

  // Reset diário da flag do motor
  if (agora.hour() == 0 && agora.minute() == 0 && agora.second() == 0) {
    motorAtivadoHoje = false;
  }

  // Controle de iluminação conforme horário e ambiente
  if (modoIluminacao == "com_luz") {
    if (agora.hour() >= 8 && agora.hour() < 20) FastLED.show();
    else FastLED.clear();
  } else {
    FastLED.show();
  }

  delay(500);  // Pequeno delay para evitar uso excessivo da CPU
}
