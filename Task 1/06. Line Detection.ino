const unsigned char Active_buzzer = 16;
void setup() {
    pinMode(4, OUTPUT); //red
    pinMode(5, OUTPUT); //green
    pinMode(Active_buzzer, OUTPUT); //buzzer
    Serial.begin(115200);   // initialize serial communication at 9600 BPS
}
void loop() {

    int sensorValue = analogRead(A0);   // read the input on analog pin 0

    float scale = sensorValue * (100.0/ 1024.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)

    Serial.println(scale);   // print out the value you read
    
    if (scale < 10.0){
     
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(Active_buzzer,LOW) 
      delay(1000); 
    
    }else{
      
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(Active_buzzer,HIGH)
      delay(1000);
    }
 

}
