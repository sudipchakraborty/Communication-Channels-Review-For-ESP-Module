
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "";
const char* password = "";

WiFiServer server(80);
WiFiClient client;

unsigned int SYS=120;
unsigned int DIA=80;
unsigned int PUL=72;
unsigned int TEMP=98;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 1000;
//____________________________________________________________________________________________________________________________________________
void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
//____________________________________________________________________________________________________________________________________________
void loop()
{
  client = server.available();    
  if (client) 
  {                                          
    String Receive_Data= Read_Data();
    Serial.println(Receive_Data);
    client.println(Get_Header());
    client.println(Get_Content());
    client.println();
    client.stop(); 
    TEMP++;
  }
}
//____________________________________________________________________________________________________________________________________________
String Read_Data(void)
{    
    String header;  
    String currentLine = "";         
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { 
      currentTime = millis();               
      if (client.available())
      {                                      
        char c = client.read();
        header += c;
        if (c == '\n')
        {      
            if (currentLine.length() == 0) 
            {
              return header;
            }
            else
            {
              currentLine = "";
            }
        } 
        else if (c != '\r') 
        {  
          currentLine += c; 
        }        
      }
    }
    return "";
}
//____________________________________________________________________________________________________________________________________________
String Get_Header(void)
{
    String resp="";
    resp="HTTP/1.1 200 OK \r\n";
    resp +="Content-type:text/html \r\n";
    resp +="Connection: close \r\n"; 
    return resp;
}
//____________________________________________________________________________________________________________________________________________
String Get_Content(void)
{
  String resp="";
  resp += "<!DOCTYPE html><html> \r\n";
  resp +="<body><h1>"+ Get_Json_Object()+"</h1> \r\n";
  resp +="</body></html> \r\n";
  return resp;
}
//____________________________________________________________________________________________________________________________________________
String Get_Json_Object(void)
{
  String jsonString;
  StaticJsonDocument<200> doc;

  doc["SYS"]  = SYS;
  doc["DIA"]  = DIA;
  doc["PUL"]  = PUL;
  doc["TEMP"] = TEMP;

  serializeJson(doc, jsonString);
  return jsonString;
}
//____________________________________________________________________________________________________________________________________________
