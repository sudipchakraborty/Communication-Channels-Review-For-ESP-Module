#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";
String serverName = "http://192.168.199.92:80/";

WiFiClient client;
HTTPClient http;
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;
StaticJsonDocument<200> Resp_Data;
//______________________________________________________________________________________________________________________________________
void setup() 
{
  Serial.begin(115200); 
  Connect_With_Router();
 }
//_____________________________________________________________________________________________________________________________________
void loop() 
{
  if (TimeOut())
  {
      String data=Get_Data_From_Server(Get_Json_Str());
      if(data !="-1")
      {
       // Serial.println(data);
        Parse_Receive_Data(data); 
        Do_Action();
      } 
      else
      {
         Serial.println("ERROR!!");
      }   
  }
}
//_____________________________________________________________________________________________________________________________________
void Do_Action(void)
{
  // Access JSON elements
  // Example: Accessing a value
  int value = Resp_Data["SYS"]; // Replace "key" with your JSON key
  Serial.println(value);
}
//_____________________________________________________________________________________________________________________________________
String Get_Data_From_Server(String pkt)
{
  pkt=serverName + "?"+pkt;
  String payload="";
  if(WiFi.status()== WL_CONNECTED)
    {
        http.begin(client,pkt.c_str());      
        int httpResponseCode = http.GET(); 
        if (httpResponseCode>0) 
        {
          // Serial.println("Response Code=");
          // Serial.println(httpResponseCode);
          payload = http.getString();
        } 
        else
        {
          // Serial.println("Error Response Code=");
          // Serial.println(httpResponseCode);
        }      
        http.end();
    }
    else 
    {
      Serial.println("WiFi Disconnected");
    }
    return payload;    
}
//_____________________________________________________________________________________________________________________________________
bool TimeOut(void)
{
  if ((millis() - lastTime) > timerDelay) 
    {
      lastTime = millis();
      return true;
    }
    else 
    {
      return false;
    }
}
//_____________________________________________________________________________________________________________________________________
void Connect_With_Router(void)
{
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());
}
//_____________________________________________________________________________________________________________________________________
String Get_Json_Str(void)
{
  String jsonString;
  StaticJsonDocument<200> doc;

  doc["cmd"]  = "bp_data";
  doc["param"]  = "";

  serializeJson(doc, jsonString);
  return jsonString;
}
//_____________________________________________________________________________________________________________________________________
void Parse_Receive_Data(String payload)
{
    int i1 = payload.indexOf('{');
    int i2 = payload.indexOf("}");
    String subString2 = payload.substring(i1, i2+1);
    DeserializationError error = deserializeJson(Resp_Data, subString2);

    if (error)
    {
      // Serial.print("JSON parsing failed: ");
      // Serial.println(error.c_str());
    } 
    else 
    {
    }
}
//_____________________________________________________________________________________________________________________________________
