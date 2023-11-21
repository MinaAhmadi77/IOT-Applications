#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <string.h>
#include <Servo.h>

const int trigPin = 5;  //D1
const int echoPin = 4;  //D2
//const int doorLED = 16; //TX
const int SS_PIN = 2;
const int RST_PIN = 0;
const int RoomLED = 15;//D8
const char *ssid     = "Mehdi";
const char *password = "Impossible";

const char* broker = "test.mosquitto.org"; //IP address of machine on which broker is installed
const int port = 1883;
const char* mqttUser = "user";
const char* mqttPassword = "user";

int distance;
long duration;
int entranceTime = 7;
int exitTime = 23;
int LdrValue = 0;
int brightness = 75;
boolean doorOpen = false;
WiFiClient espClient;
PubSubClient client(espClient);
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo servo;






void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message received in topic: ");
  Serial.print(topic);
  
  Serial.print("   length is:");
  Serial.println(length);

  Serial.print("Data Received From Broker:");
  String data = "";
  
  
  for (int i = 0; i < length; i++) {
    
      data += String((char)payload[i]);
  }
  
  Serial.println(data);
  Serial.println("hereeeee");
  Serial.println(strcmp(topic,"smartOffice"));
  if(strcmp(topic,"smartOffice") == 0){
    brightness = data.toInt();
  }

  if(strcmp(topic,"smartOffice/lightsOnTime") == 0){
    exitTime = data.toInt();
    
  }
  if(strcmp(topic,"smartOffice/lightsOffTime") == 0){
    entranceTime = data.toInt();
  }
  Serial.println();
  Serial.println("-----------------------");
  Serial.println();

}

void MQTTPOST()
{
//payload formation begins here
String payload ="10";
char attributes[1000];
payload.toCharArray( attributes, 1000 );
client.publish("test", attributes); //topic="test" MQTT data post command.
Serial.println( attributes );
}






void setup() {
  Serial.begin(115200); // Starts the serial communication


  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
//  pinMode(doorLED, OUTPUT);
  pinMode(RoomLED,OUTPUT);
  
  digitalWrite(RoomLED,HIGH);
//  digitalWrite(doorLED,LOW);
  servo.attach(16);
  servo.write(180);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(WiFi.localIP());

  client.setServer(broker, port);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {

      Serial.println("connected to MQTT broker");

    }
    else
    {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(500);

    }
  }
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  timeClient.setTimeOffset(12600);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  client.subscribe("smartOffice");
  client.subscribe("smartOffice/lightsOnTime");
  client.subscribe("smartOffice/lightsOffTime");
  
}

void loop() {
  client.loop();
  timeClient.update();
  int currentHour = timeClient.getHours();
//  Serial.print("Hour: ");
//  Serial.println(currentHour); 
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
//  Serial.println(brightness);
  duration = pulseIn(echoPin, HIGH);
  LdrValue = analogRead(A0);
  float voltage = LdrValue * (5.0/1023.0);
//  Serial.println(voltage);
  if(doorOpen){
    servo.write(180);
    if(voltage * 51 < brightness)
    {
      analogWrite(RoomLED, brightness -(voltage)*51);
    }
  else{
      analogWrite(RoomLED, 0);
    }
    
  }
  else{
    servo.write(0);
    analogWrite(RoomLED, 0);
  }
  
  
  // Calculating the distance
  distance = duration * 0.034 / 2;


  if (distance < 10 && currentHour<=exitTime && currentHour >= entranceTime ){
//    digitalWrite(doorLED,HIGH);
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
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
  doorOpen = !doorOpen;
  
//  client.publish("test", "1");
  
//  MQTTPOST();
  delay(1000);
  Serial.println();
  }
  else{
//    digitalWrite(doorLED,LOW);
  }

}
