#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//SSID and Password of your wifi router
const char* ssid = "Moto";
const char* password = "reset1234";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();          //Handle client requests
}
