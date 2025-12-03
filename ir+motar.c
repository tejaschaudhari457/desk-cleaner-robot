// ---------------- Motor Pins (PWM) ----------------
// Motor 1 (Right side)
int IN1 = 5;   // PWM
int IN2 = 6;   // PWM

// Motor 2 (Left side)
int IN3 = 9;   // PWM
int IN4 = 10;  // PWM

// ---------------- IR Sensor Pins ------------
int irPinRight = 8;   // Right IR sensor
int irPinLeft  = 7;   // Left IR sensor  (use pin 7 for sensor)

// ---------- Speed Settings (0–255) ----------
int forwardSpeed = 180;   // speed for straight motion
int turnSpeed    = 150;   // speed when turning

void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // IR sensor pins
  pinMode(irPinRight, INPUT);
  pinMode(irPinLeft,  INPUT);

  Serial.begin(9600);
}

void loop() {

  int rightIR = digitalRead(irPinRight); // 0 = obstacle, 1 = clear
  int leftIR  = digitalRead(irPinLeft);  // 0 = obstacle, 1 = clear

  Serial.print("Right IR = ");
  Serial.print(rightIR);
  Serial.print(" | Left IR = ");
  Serial.println(leftIR);

  // ----------------- LOGIC -----------------
  if (rightIR == 0 && leftIR == 0) {
    stopMotors();
    Serial.println("Both Detect → STOP");
  }
  else if (rightIR == 0 && leftIR == 1) {
    turnLeftSmooth();
    Serial.println("Right Detect → TURN LEFT (smooth)");
  }
  else if (rightIR == 1 && leftIR == 0) {
    turnRightSmooth();
    Serial.println("Left Detect → TURN RIGHT (smooth)");
  }
  else {
    moveForwardSmooth();
    Serial.println("No Obstacle → FORWARD (smooth)");
  }

  Serial.println("-----------------------");
  delay(100);
}

// -------------- Motor Helper Functions --------------

void moveForwardSmooth() {
  // Right motor forward
  analogWrite(IN1, forwardSpeed);  // speed
  digitalWrite(IN2, LOW);          // direction

  // Left motor forward
  analogWrite(IN3, forwardSpeed);  // speed
  digitalWrite(IN4, LOW);          // direction
}

void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// Right sensor detects → turn left smoothly
void turnLeftSmooth() {
  // Right motor stop
  analogWrite(IN1, 0);
  digitalWrite(IN2, LOW);

  // Left motor forward at turn speed
  analogWrite(IN3, turnSpeed);
  digitalWrite(IN4, LOW);
}

// Left sensor detects → turn right smoothly
void turnRightSmooth() {
  // Right motor forward at turn speed
  analogWrite(IN1, turnSpeed);
  digitalWrite(IN2, LOW);

  // Left motor stop
  analogWrite(IN3, 0);
  digitalWrite(IN4, LOW);
}
