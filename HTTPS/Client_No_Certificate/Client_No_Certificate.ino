#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

const char* ssid = "";
const char* password = "";
String cmd="";
//________________________________________________________________________________________________________________________________________
void setup() 
{
  Serial.begin(115200);
  connect_with_Wifi();
}
//________________________________________________________________________________________________________________________________________
void loop() 
{
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    String payload=Get_Data_From_Server(cmd);
    Serial.println(payload);
  }
  Serial.println();
  Serial.println("Waiting 1 sec. before the next round...");
  delay(1000);
}
//________________________________________________________________________________________________________________________________________
void connect_with_Wifi(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
}
//________________________________________________________________________________________________________________________________________
String Get_Data_From_Server(String cmd)
{
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if (https.begin(*client,cmd))
    {  
      int httpCode = https.GET();
      if (httpCode > 0) 
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
        {
          String payload = https.getString();
          return payload;
        }
      } else 
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } 
    else 
    {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
    return "";
}
//________________________________________________________________________________________________________________________________________










