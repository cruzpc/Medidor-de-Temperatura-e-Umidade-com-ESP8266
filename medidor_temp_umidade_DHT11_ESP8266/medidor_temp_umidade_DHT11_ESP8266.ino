#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11

// colocar a SSID e a senha da rede que o ESP8266 irá conectar:
const char* ssid = "net ap 302";  // Enter SSID here
const char* password = "5859abcd";  //Enter Password here

// cria as variáveis globais para armazenar os valores de temperatura e umidade
float temperatura;
float umidade;
float Temperature;
float Humidity;

ESP8266WebServer server(80);

// DHT Sensor
#define DHTPin 5 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

void setup() {
  
  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);

  dht.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  
  server.handleClient();

  //temperatura = dht.readTemperature();
  //umidade = dht.readHumidy();
  

  Serial.print("Temperatura:");
  Serial.println(Temperature);
  Serial.print("Umidade:");
  Serial.println(Humidity);
  delay(3000);
  
}

void handle_OnConnect() {

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 

}

void handle_NotFound(){

  server.send(404, "text/plain", "Not found");

}

String SendHTML(float Temperaturestat,float Humiditystat){
  
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
  ptr +=(int)Temperaturestat;
  ptr +=" C</p>";
  ptr +="<p>Umidade: ";
  ptr +=(int)Humiditystat;
  ptr +=" %</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>";
  
  return ptr;
}
