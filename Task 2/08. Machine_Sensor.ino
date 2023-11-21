#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>



const int trigPin = 5;  //D1
const int echoPin = 4;  //D2
const int buzzer = 0;  //D3
const char* ssid = "Mehdi";
const char* password = "Impossible";

AsyncWebServer server(80);

// defines variables
long duration;
int distance;


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>

<body>
<center>

  <h2>UltraSonic</h2>
  <p id="valueOfslide">50</p>
  <p><input type="range" style="width:50rem;"  id="Slider" min="0" max="180" value="0" step="1"></p>
<script>

setInterval(function() 
{
  getData();
}, 100); 
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    
      if (this.responseText!=="") {
        document.getElementById("Slider").value =
        this.responseText;
        document.getElementById("valueOfslide").innerHTML =
        this.responseText;
        console.log(this.responseText);
      }
 
  };
  xhttp.open("GET", "ultraSonic", true);
  xhttp.send();
}
</script>

</center>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(buzzer, OUTPUT); // Sets the trigPin as an Output
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
    });
  server.on("/ultraSonic", HTTP_GET, [](AsyncWebServerRequest *request){
     // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;
  if(distance < 10){
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(buzzer, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(buzzer, LOW);
    
  }
  // Prints the distance on the Serial Monitor
  String sensor_value = String(distance);
  Serial.println(distance);
   request->send(200, "text/plane", sensor_value);
  });
  server.begin();
  
}




void loop() {

}
