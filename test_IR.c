int irPin = 7;   // IR sensor output pin

void setup() {
  pinMode(irPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(irPin);

  if (sensorValue == 0) { 
    // Most IR sensors give LOW when obstacle detected
    Serial.println("Obstacle Detected!");
  } else {
    Serial.println("No Obstacle");
  }

  delay(500);
}
