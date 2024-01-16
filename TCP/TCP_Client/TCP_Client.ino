#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *ssid = "";
const char *password = "";
const char *serverIP = "192.168.199.43";
const int serverPort = 1234;
long lastTime=0;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to the server
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");
    // Send data to the server
    client.print("Hello, server!");
  } else {
    Serial.println("Connection failed");
  }
}

void loop() {
  // Check if data is available from the server
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // Send data to the server periodically
  if (millis() - lastTime > 5000) {
    client.print("Sending data to server...");
    lastTime = millis();
  }
}
