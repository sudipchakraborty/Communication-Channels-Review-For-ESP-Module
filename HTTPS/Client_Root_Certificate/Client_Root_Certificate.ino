/*
  Complete project details: https://RandomNerdTutorials.com/esp8266-nodemcu-https-requests/
  Based on the example created by Ivan Grokhotkov, 2015 (File > Examples > ESP8266WiFi > HTTPSRequests)
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

// Root certificate for howsmyssl.com
const char IRG_Root_X1 [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)CERT";

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Create a list of certificates with the server certificate
X509List cert(IRG_Root_X1);

void setup() {
  Serial.begin(115200);
  //Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  //Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void loop() {
  WiFiClientSecure client;

  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    client.setTrustAnchors(&cert);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(client, "https://simulab.in/wp-json/rsh/v1")) {  // HTTPS     //https://www.howsmyssl.com/a/check"

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  Serial.println();
  Serial.println("Waiting 2min before the next round...");
  delay(10000);
}