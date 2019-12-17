// Import required libraries
#include "ESP8266WiFi.h"
#include "DHT.h"
#include <ESP8266WebServer.h>
#include "ArduinoJson.h"

// Sensor pins
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// WiFi parameters
const char* ssid = "NETIASPOT-2.4GHzEXT";
const char* password = "vaR8xAe8";
#define LISTEN_PORT           80

//WiFiServer server(LISTEN_PORT);
ESP8266WebServer server(80);

// Variables to be exposed to the API
float temperature;
float humidity;
float analog;

void setup(void)
{
  // Start Serial
  Serial.begin(115200);
  pinMode(A0, INPUT);

  // Init DHT
  dht.begin();
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.on("/temperature", readTemperature);
  server.on("/analog", readAnalog);
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void readAnalog() {
  analog = analogRead(A0);
  const int capacity = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<capacity> doc;
  doc["module"] = "main";
  doc["analog"] = analog;
  String json;
  serializeJson(doc, json);
  server.send(200, "text/plain", json);
}

void readTemperature() {
    temperature = dht.readTemperature(); 
    const int capacity = JSON_OBJECT_SIZE(3);
    StaticJsonDocument<capacity> doc;
    doc["module"] = "main";
    doc["temperature"] = temperature;
    String json;
    serializeJson(doc, json);
    server.send(200, "text/plain", String(temperature));
}

void readHumidity() {
    humidity = dht.readHumidity();
    server.send(200, "text/plain", String(humidity));
}

void loop() {
  server.handleClient();
}
