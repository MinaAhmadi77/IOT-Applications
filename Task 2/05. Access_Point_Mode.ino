#include <ESP8266WiFi.h>

const char* ssid     = "ESP8266-Access-Point";
const char* password = "123456789";

WiFiServer server(80);
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup(void){  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  Serial.println("assigning SSID ...");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed");
  Serial.println("IP before configuration:");
  Serial.println(WiFi.softAPIP());
  Serial.println("Setting soft_AP configuration ...");
  Serial.println(WiFi.softAPConfig(local_IP,gateway,subnet) ? "Ready" : "Failed");
  server.begin();
  Serial.println("IP after configuration: " );
  Serial.println(WiFi.softAPIP());

}

void loop(void){
}
