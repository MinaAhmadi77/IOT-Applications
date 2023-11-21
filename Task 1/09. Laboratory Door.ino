#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


// Replace with your network credentials
const char *ssid     = "Mehdi";
const char *password = "Impossible";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
const int ledPin = 0; 
const unsigned char Active_buzzer = 16;
Servo servo;
void setup() 
{
  servo.attach(2); //D4
  servo.write(0);
  delay(2000);
  pinMode(Active_buzzer, OUTPUT); 
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  timeClient.setTimeOffset(12600);
}
  
  SPI.begin();      // Initiate  SPI bus
  
  mfrc522.PCD_Init();   // Initiate MFRC522
}
void loop() 
{
  timeClient.update();
  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour); 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "49 1D D7 A2") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    if(currentHour >= 9 && currentHour <= 15)
    {
      
    Serial.println(" Welcome ");
    delay(1000);
    Serial.println();
    servo.write(180);
    digitalWrite(ledPin,HIGH);
    delay(3000);
    servo.write(0);
    digitalWrite(ledPin,LOW);
    delay(1000);
    statuss = 1;
      
    }
    else{
      Serial.println(" not in right time ");
      digitalWrite(Active_buzzer,HIGH);
      delay(3000);
      digitalWrite(Active_buzzer,LOW);
    }
    
    
  }
  
  else   {
    Serial.println(" Access Denied ");
    digitalWrite(Active_buzzer,HIGH);
    delay(3000);
    digitalWrite(Active_buzzer,LOW);
  }
} 
