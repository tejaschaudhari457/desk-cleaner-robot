// ---------------- Motor Pins (PWM) ----------------
int IN1 = 5;   // Right motor
int IN2 = 6;
int IN3 = 9;   // Left motor
int IN4 = 10;

// ---------------- IR Sensor Pins ------------------
int irPinRight = 8;
int irPinLeft  = 7;

// ---------------- Ultrasonic Pins -----------------
#define TRIG 3
#define ECHO 4

// ---------- Speed Settings ----------
int forwardSpeed = 180;
int turnSpeed    = 150;
int backSpeed    = 180;

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(irPinRight, INPUT);
  pinMode(irPinLeft, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {

  // ---------------- Read IR Sensors ----------------
  int rightIR = digitalRead(irPinRight);
  int leftIR  = digitalRead(irPinLeft);

  // ---------------- Read Ultrasonic ----------------
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2;   // cm

  Serial.print("Right IR: ");
  Serial.print(rightIR);
  Serial.print(" | Left IR: ");
  Serial.print(leftIR);
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // -------------------------------------------------
  //                   MAIN LOGIC
  // -------------------------------------------------

  // 1️⃣ ULTRASONIC HIGH PRIORITY → BACKWARD
  if (distance < 15 && distance > 0) {
    moveBackwardSmooth();
    Serial.println("Ultrasonic: Close Object → BACKWARD");
  }

  // 2️⃣ BOTH IR DETECT OBSTACLE → STOP
  else if (rightIR == 0 && leftIR == 0) {
    stopMotors();
    Serial.println("Both IR Detect → STOP");
  }

  // 3️⃣ RIGHT IR DETECT → TURN LEFT
  else if (rightIR == 0 && leftIR == 1) {
    turnLeftSmooth();
    Serial.println("Right IR Detect → TURN LEFT");
  }

  // 4️⃣ LEFT IR DETECT → TURN RIGHT
  else if (rightIR == 1 && leftIR == 0) {
    turnRightSmooth();
    Serial.println("Left IR Detect → TURN RIGHT");
  }

  // 5️⃣ NO OBSTACLE → FORWARD
  else {
    moveForwardSmooth();
    Serial.println("Clear → FORWARD");
  }

  Serial.println("--------------------------------");
  delay(100);
}

// ---------------- Motor Functions ----------------

// Forward
void moveForwardSmooth() {
  analogWrite(IN1, forwardSpeed);
  digitalWrite(IN2, LOW);

  analogWrite(IN3, forwardSpeed);
  digitalWrite(IN4, LOW);
}

// Backward
void moveBackwardSmooth() {
  digitalWrite(IN1, LOW);
  analogWrite(IN2, backSpeed);

  digitalWrite(IN3, LOW);
  analogWrite(IN4, backSpeed);
}

// Stop
void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// Turn Left
void turnLeftSmooth() {
  analogWrite(IN1, 0);
  digitalWrite(IN2, LOW);

  analogWrite(IN3, turnSpeed);
  digitalWrite(IN4, LOW);
}

// Turn Right
void turnRightSmooth() {
  analogWrite(IN1, turnSpeed);
  digitalWrite(IN2, LOW);

  analogWrite(IN3, 0);
  digitalWrite(IN4, LOW);
}
