/*
  TESTE – PÁGINA WEB COM INFORMAÇÕES DOS PEIXES
  Integrantes: Matheus Muller, Germano Lagana e Vinicius Padilha

  Este teste cria uma rede Wi-Fi local chamada "AQUARIO_TESTE".
  Ao acessar pelo navegador, o usuário verá as preferências de cada peixe:
  - Luz ideal
  - Faixa de temperatura
  - Frequência de alimentação
*/

#include <WiFi.h>
#include <WebServer.h>

// ======= CONFIGURAÇÃO DO Wi-Fi =======
const char* ssid = "AQUARIO_TESTE";  // Nome da rede
const char* senha = "aquario123";    // Senha da rede

WebServer server(80);  // Servidor HTTP na porta 80

// ======= HTML da Página com as Informações dos Peixes =======
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
  </body></html>)rawliteral";

  server.send(200, "text/html", html);  // Envia a página ao navegador
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, senha);  // Cria rede Wi-Fi
  Serial.println("Rede criada!");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());  // Mostra IP da rede

  server.on("/", paginaWeb);  // Define a página principal
  server.begin();             // Inicia o servidor
}

void loop() {
  server.handleClient();  // Atende as requisições HTTP
}
