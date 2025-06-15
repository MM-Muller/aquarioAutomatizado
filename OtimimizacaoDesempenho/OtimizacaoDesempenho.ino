/**************************************
 * 1. OTIMIZAÇÃO DE TEMPO DE EXECUÇÃO
 **************************************/

// Uso de millis() ao invés de delay() para alternar telas sem travar o sistema
unsigned long tempoAnterior = 0;
int telaAtual = 0;

if (millis() - tempoAnterior > 5000) {
  telaAtual = (telaAtual + 1) % 3;
  tempoAnterior = millis();
  atualizarLCD(agora); // Atualiza apenas a cada 5s
}
 
/**************************************
 * 2. USO DE PROTOCOLOS EFICIENTES
 **************************************/

// Comunicação I2C para LCD e RTC → usa apenas 2 fios (pinos 21 e 22)
// Reduz uso de pinos e facilita expansão
Wire.begin(21, 22); // I2C otimizado

/**************************************
 * 3. CONTROLE DE MOTOR EFICIENTE
 **************************************/

// Evita múltiplas ativações com flag diária
bool motorAtivadoHoje = false;

if (hora_certa && !motorAtivadoHoje) {
  motor.step(512); // apenas 1/4 de volta
  desligarPinosMotor(); // desliga pinos após uso → menor consumo
  motorAtivadoHoje = true;
}

if (hora == 0 && minuto == 0 && segundo == 0) {
  motorAtivadoHoje = false; // reinicia flag à meia-noite
}

/**************************************
 * 4. EFICIÊNCIA NA ILUMINAÇÃO (LED)
 **************************************/

// Controle de iluminação por horário
if (modoIluminacao == "com_luz") {
  if (hora >= 8 && hora < 20) FastLED.show(); // ativa LED só durante o dia
  else FastLED.clear();                       // apaga LED à noite
} else {
  FastLED.show(); // sempre ligado se for ambiente sem luz
}

/**************************************
 * 5. EFICIÊNCIA NA LÓGICA WEB
 **************************************/

// WebServer leve, sem dependência de arquivos externos
server.on("/", paginaWeb);
server.on("/config", HTTP_GET, []() {
  // Aplica apenas se argumentos forem recebidos
  if (server.hasArg("peixe")) configurarPeixe(server.arg("peixe"));
  if (server.hasArg("ambiente")) modoIluminacao = server.arg("ambiente");

  // Resposta enxuta e sem overhead
  server.send(200, "text/plain", "Configurações atualizadas");
});

/**************************************
 * 6. SENSOR DE TEMPERATURA – USO RÁPIDO
 **************************************/

// Solicita temperatura apenas de um sensor (index 0)
sensors.requestTemperatures();
temperaturaAtual = sensors.getTempCByIndex(0); // leitura direta e rápida

/**************************************
 * 7. ECONOMIA DE MEMÓRIA E VARIÁVEIS FIXAS
 **************************************/

// Horários de alimentação com matriz fixa (máximo 3)
int horarios[3][2] = {{8, 0}, {20, 0}, {23, 0}};

// Uso de buffer char evita concatenação lenta de Strings
char buffer[6];
sprintf(buffer, "%02d:%02d", hora, minuto);

/**************************************
 * 8. DEBUG RESTRITO
 **************************************/

// Apenas ativa o serial se necessário para testes
Serial.begin(115200); // Evitar manter Serial ativa em produção

/**************************************
 * 9. USO DE BIBLIOTECAS OTIMIZADAS
 **************************************/

// FastLED: controle de LEDs eficiente com baixo overhead
// DallasTemperature: leitura do sensor DS18B20 otimizada via OneWire
// RTClib: leitura de hora com baixa latência
// LiquidCrystal_I2C: reduz uso de pinos para o LCD

