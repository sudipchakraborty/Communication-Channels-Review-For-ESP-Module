#include <ESP8266WiFi.h>
const char* ssid = "";
const char* password = "";
int LED_TCP = 2; // GPIO2 of ESP8266
WiFiServer wifiServer(5045);

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
  wifiServer.begin();
}

void loop() {
  WiFiClient client = wifiServer.available();
  if (client) {
    while (client.connected()) {
      while (client.available() > 0) {
        char c = client.read();
        Serial.write(c);
        delay(10);

        if (Serial.available()) {
          size_t len = Serial.available();
          uint8_t sbuf[len];
          Serial.readBytes(sbuf, len);
          client.write(sbuf, len);
          delay(1);
        }
      }
    }
  }
  client.stop();
}