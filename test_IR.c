int irPin1 = 8;   // IR Sensor 1 pin  right side 
int irPin2 = 9;   // IR Sensor 2 pin   left side 

void setup() {
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);

  Serial.begin(9600);
}

void loop() {

  int sensorValue1 = digitalRead(irPin1);
  int sensorValue2 = digitalRead(irPin2);

  // ----- Sensor 1 -----
  if (sensorValue1 == 0) { 
    Serial.println("IR Sensor 1: Obstacle Detected!");
  } else {
    Serial.println("IR Sensor 1: No Obstacle");
  }

  // ----- Sensor 2 -----
  if (sensorValue2 == 0) { 
    Serial.println("IR Sensor 2: Obstacle Detected!");
  } else {
    Serial.println("IR Sensor 2: No Obstacle");
  }

  Serial.println("------------------------");
  delay(500);
}
