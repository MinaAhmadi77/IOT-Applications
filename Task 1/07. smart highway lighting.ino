int rows = 5;
int cols = 2;
int ledPins[5][2] = {{16, 16},{3, 1}, {13, 15}, {14, 12},{0, 2}};
int max_distance = 14;
int min_distance = 3;
int pinCount = 10;
const int trigPin = 5;  //D1
const int echoPin = 4;  //D2

// defines variables
long duration;
int distance;

void setup() {
  int i = 0;
  int j = 0;
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      pinMode(ledPins[i][j], OUTPUT);
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
//  pinMode(9,OUTPUT);
//  digitalWrite(9,HIGH);
//  break;
}

void loop() {

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
  // Prints the distance on the Serial Monitor
  
  
  int line = (distance * 4) / (max_distance - min_distance + 1);
  Serial.println(distance);
  Serial.println(line);
  int i = 0;
  int j = 0;
  for (i = 0 ; i < cols ; i++) {

    digitalWrite(ledPins[line][i], HIGH);

 }
 delay(5000);
 for (i = 0 ; i < rows; i++)
  for(j = 0 ; j < cols ; j++)
    digitalWrite(ledPins[i][j], LOW);

  delay(2000);
}
