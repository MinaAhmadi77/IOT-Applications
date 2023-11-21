
#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);
//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>Kiana & Erfan</h1><br>
 <a href="play"><button type="button">play</button></a><br>
 <a href="pause"><button type="button">pause</button></a><br>
 <a href="volumeUp"><button type="button">+</button></a><br>
 <a href="volumeDown"><button type="button">-</button></a><br>
 <a href="name"><button type="button">name</button></a><br>
 
<hr>

</center>

</body>
</html>
)=====";
const char MOVIE1_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>ekhrajiha</h1><br>
<img src = "https://www.uptvs.com/wp-contents/uploads/2020/04/59580.jpg" alt = "ekhrajiha"/>
<hr>

</center>

</body>
</html>
)=====";
const char MOVIE2_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>samte khoda</h1><br>
<img src = "https://app.akharinkhabar.ir/images/2018/07/21/9351c995-0faf-412a-8c11-c64c46ddde1f.jpeg" alt = "samte khoda"/>
<hr>

</center>

</body>
</html>
)=====";



//SSID and Password of your WiFi router
const char* ssid = "Mehdi";
const char* password = "Impossible";


ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", MAIN_page); //Send web page
}

void handlePlay() { 
 Serial.println("play");
 
 server.send(200, "text/html", MAIN_page); //Send ADC value only to client ajax request
}

void handlePause() { 
 Serial.println("pause");
 server.send(200, "text/html", MAIN_page); //Send ADC value only to client ajax request
}

void handleVolumeDown() { 
 Serial.println("volumeDown");
 server.send(200, "text/html", MAIN_page); //Send ADC value only to client ajax request
}
void handleVolumeUp() { 
 Serial.println("volumeUp");
 server.send(200, "text/html", MAIN_page); //Send ADC value only to client ajax request
}
void handleName(){
  while (true){
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
  if (content.substring(1) == "49 1D D7 A2")
  {
      server.send(200, "text/html", MOVIE1_page);
      break;
  }
  else{
    server.send(200, "text/html", MOVIE2_page);
    break;
  }
  
  
  }

}

void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/play", handlePlay); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/pause", handlePause);
  server.on("/volumeUp", handleVolumeUp); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/volumeDown", handleVolumeDown);
  server.on("/name", handleName);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();          //Handle client requests
}
