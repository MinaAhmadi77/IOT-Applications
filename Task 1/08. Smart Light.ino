const int ledPin = 2; 

void setup() {

  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  
}

void loop() {

  int sensorValue = analogRead(A0);
  delay(1000);
  float dutyCycle = sensorValue * (256.0/1024.0);
  Serial.println(dutyCycle);
  analogWrite(ledPin, dutyCycle);
  
}
