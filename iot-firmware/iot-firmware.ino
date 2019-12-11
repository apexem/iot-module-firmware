// Import required libraries
#include "ESP8266WiFi.h"
#include <aREST.h>
#include "DHT.h"

// Sensor pins
#define DHTPIN 2
#define DHTTYPE DHT11
//#define TRIGPIN 15
//#define ECHOPIN 13

// Create aREST instance
aREST rest = aREST();

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// WiFi parameters
const char* ssid = "NETIASPOT-2.4GHzEXT";
const char* password = "vaR8xAe8";

//const char* ssid = "NETIASPOT-2.4GHz-C4y6";
//const char* password = "xTUD5dMN";

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
float temperature;
float humidity;
float analog;

//float distance;
//float duration;

void setup(void)
{
  // Start Serial
  Serial.begin(115200);
  pinMode(A0, INPUT);

  // Init DHT
  dht.begin();

  // Init variables and expose them to REST API
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);
  rest.variable("analog", &analog);

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("dht_module");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Reading temperature and humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  analog = analogRead(A0);
  rest.handle(client);
}
