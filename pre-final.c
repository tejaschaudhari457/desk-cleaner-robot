//Very simple: “If anything is close or edge detected → back + turn left, else go forward.”
//Uses analog IR as ground/edge detection, single reaction.

// ---------------- Motor Pins (PWM) ----------------
int IN1 = 5;   // Right motor
int IN2 = 6;
int IN3 = 9;   // Left motor
int IN4 = 10;

// ---------------- IR Sensor Pins ------------------
int irPinRight = 8;   // Digital IR Right
int irPinLeft  = 7;   // Digital IR Left

// ---------------- Ultrasonic Pins -----------------
#define TRIG 3
#define ECHO 4

// ---------- Speed Settings ----------
int forwardSpeed = 180;
int backSpeed    = 180;
int turnSpeed    = 160;

// ---------- Distance Threshold (cm) ----------
float obstacleDist = 5.0;   // Same style as Code 1 (very close)

// --------------------------------------------------
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

// --------------------------------------------------
void loop() {

  // --------- Read IR Sensors (Digital) -----------
  int rightIR = digitalRead(irPinRight);  // 0 = obstacle, 1 = clear
  int leftIR  = digitalRead(irPinLeft);   // 0 = obstacle, 1 = clear

  // --------- Read Ultrasonic Distance -----------
  float distance = getDistanceCm();

  Serial.print("Right IR: ");
  Serial.print(rightIR);
  Serial.print(" | Left IR: ");
  Serial.print(leftIR);
  Serial.print(" | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // -------------------------------------------------
  //           MAIN LOGIC (SAME STYLE AS CODE 1)
  // If:
  //   - Distance < 5 cm (very close obstacle), OR
  //   - Any IR triggered (0 = obstacle / edge)
  // Then:
  //   Stop -> Backward -> Turn Left
  // Else:
  //   Forward
  // -------------------------------------------------

  if ( (distance > 0 && distance < obstacleDist) ||
       (rightIR == 0) ||
       (leftIR == 0) ) {

    // Stop
    stopMotors();
    delay(200);

    // Go Backwards
    moveBackward();
    delay(1000);

    // Turn Left (Right motor forward, Left motor backward)
    turnLeft();
    delay(1000);

    // Stop after manoeuvre (optional)
    stopMotors();
  }
  else {
    // Forward
    moveForward();
  }

  delay(50);
}

// ================= Helper Functions =================

// Measure distance in cm (like mdistance() in Code 1, but fixed)
float getDistanceCm() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); // timeout 30ms
  if (duration == 0) {
    return -1;  // no echo
  }

  float distance = duration * 0.034 / 2.0; // in cm
  return distance;
}

// -------- Motor: Forward --------
void moveForward() {
  // Right motor forward
  analogWrite(IN1, forwardSpeed);
  digitalWrite(IN2, LOW);

  // Left motor forward
  analogWrite(IN3, forwardSpeed);
  digitalWrite(IN4, LOW);
}

// -------- Motor: Backward --------
void moveBackward() {
  // Right motor backward
  digitalWrite(IN1, LOW);
  analogWrite(IN2, backSpeed);

  // Left motor backward
  digitalWrite(IN3, LOW);
  analogWrite(IN4, backSpeed);
}

// -------- Motor: Stop --------
void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// -------- Motor: Turn Left (like Code 1: R FWD, L BWD) --------
void turnLeft() {
  // Right motor forward
  analogWrite(IN1, turnSpeed);
  digitalWrite(IN2, LOW);

  // Left motor backward
  digitalWrite(IN3, LOW);
  analogWrite(IN4, turnSpeed);
}
