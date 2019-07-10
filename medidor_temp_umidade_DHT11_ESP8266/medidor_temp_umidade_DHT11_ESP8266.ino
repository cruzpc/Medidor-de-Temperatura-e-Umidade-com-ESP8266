#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

// Define pino do sensor
#define pinoDHT 5 

// colocar a SSID e a senha da rede que o ESP8266 irá conectar:
const char* ssid = "SAUL";
const char* senha = "saul1234"; 

// cria as variáveis globais para armazenar os valores de temperatura e umidade
float temperatura;
float umidade;

// inicializa o servidor http no ESP8266
ESP8266WebServer server(80);
               
// Inicializa o sensor, cria um objeto DHT dht
DHT dht(pinoDHT, DHT11);                

void setup() {

  // velocidade da conexão serial
  Serial.begin(115200);
  delay(100);
  
  pinMode(pinoDHT, INPUT);

  dht.begin();              

  Serial.print("Conectando a: ");
  Serial.println(ssid);

  //conecta a rede
  WiFi.begin(ssid, senha);

  //verifica se está conectado, se não printa .... no serial
  while (WiFi.status() != WL_CONNECTED) {
    delay(800);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi CONECTADO!!!");
  Serial.print("IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("Servido HTTP iniciado!");

}
void loop() {
  
  server.handleClient();

  
}

void handle_OnConnect() {

  temperatura = dht.readTemperature(); // pega valor de temperatura do sensor DHT e guarda na variavel temperatura
  umidade = dht.readHumidity(); // pega valor de umidade do sensor DHT e guarda na variavel umidade 
  server.send(200, "text/html", SendHTML(temperatura,umidade)); 
  Serial.print("Temperatura:");
  Serial.println(temperatura);
  Serial.print("Umidade:");
  Serial.println(umidade);
}

void handle_NotFound(){

  server.send(404, "text/plain", "404 Não Encontrado");

}

String SendHTML(float Temperaturastat,float Umidadestat){
  
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Medidor de temperatura e umidade</h1>\n";
  ptr +="<p>Temperatura: ";
  ptr +=(int)Temperaturastat;
  ptr +=" &deg;C</p>";
  ptr +="<p>Umidade: ";
  ptr +=(int)Umidadestat;
  ptr +=" %</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>";
  
  return ptr;
}
