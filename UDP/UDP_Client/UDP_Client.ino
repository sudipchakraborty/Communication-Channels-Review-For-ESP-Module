#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "";
const char* password = "";

const char * udpAddress = "192.168.199.92"; // enter your ip destination ip here
WiFiUDP udp;
unsigned int udpPort=8888;  //udp port to connect

char pktbuf1[10];
char rx_val;
void setup() {
Serial.begin(115200);
WiFi.begin(ssid, password);
while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
   // ESP.restart();
  }
  Serial.println(WiFi.localIP());
  Serial.println("Status: Connected");

  /* Enable udp */
  udp.begin(udpPort);
  Serial.println(udpPort);
}

void loop() { 
  int rp1=udp.parsePacket();
 if(!rp1)
 { 
   if (Serial.available()>0)
   {
     rx_val=Serial.read();
     Serial.print("udp_send: ");
     Serial.println(rx_val);
     udp.beginPacket(udpAddress,udpPort);
     udp.write(rx_val);
     udp.endPacket();
   }
 }
 else
 {
    udp.read(pktbuf1,2);
    delay(20);
    Serial.print("Packet from "+String(udpAddress)+": ");
    Serial.print(pktbuf1);
    delay(1000);
  }   
}