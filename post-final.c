// =====================================================
// DESK CLEANER ROBOT – FINAL CODE (SAFE + SMOOTH)
// -----------------------------------------------------
// FEATURES:
// 1. IR DOWNWARD SENSORS → Detect desk edge (fall protection)
// 2. Ultrasonic → Detect front obstacle
// 3. PWM Motor Driver → Smooth forward, backward, turning
// 4. Priority:
//      Edge > Obstacle > Forward
//✔ Desk edge safety
//✔ Smooth movements
//✔ Accurate obstacle detection
//✔ Simple tuning
//✔ Clean logic + clear comments
// =====================================================


// ---------------- MOTOR PINS (L293D DRIVER) ----------------
int IN1 = 5;     // Right Motor Forward (PWM)
int IN2 = 6;     // Right Motor Backward (PWM)
int IN3 = 9;     // Left Motor Forward  (PWM)
int IN4 = 10;    // Left Motor Backward (PWM)


// ---------------- IR EDGE SENSOR PINS ----------------
// These IR sensors point DOWN to detect edge/no surface
int irPinRight = 8;   // Right side IR
int irPinLeft  = 7;   // Left side IR

// ASSUMPTION:
//   0 = DESK SURFACE DETECTED
//   1 = EDGE / NO SURFACE (DANGER)
// If opposite, we can invert easily.


// ---------------- ULTRASONIC (FRONT) -----------------
#define TRIG 3
#define ECHO 4


// ---------------- MOVEMENT SPEEDS --------------------
int forwardSpeed = 170;   // Speed during forward motion
int backSpeed    = 180;   // Speed during backward motion
int turnSpeed    = 160;   // Speed during turns


// ---------------- TIMINGS FOR MOVEMENT ---------------
int backTimeEdge      = 400;   // Backup time when edge detected
int turnTimeEdge      = 500;   // Turn time after edge
int backTimeObstacle  = 250;   // Backup when obstacle detected
int turnTimeObstacle  = 350;   // Turn time for obstacle


// ---------------- DISTANCE SETTINGS -------------------
float frontObstacleDist = 12.0;   // Obstacle threshold in cm


// =====================================================
void setup() {
  Serial.begin(9600);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor pins
  pinMode(irPinRight, INPUT);
  pinMode(irPinLeft, INPUT);

  // Ultrasonic pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  stopMotors();   // Start safely
}


// =====================================================
void loop() {

  // --------- Read IR Sensors (Downward) ----------
  int rightIR = digitalRead(irPinRight);
  int leftIR  = digitalRead(irPinLeft);

  // Convert digital reading into edge detection
  bool edgeRight = (rightIR == 1);  
  bool edgeLeft  = (leftIR == 1);   

  // If your IR is opposite, use:
  // bool edgeRight = (rightIR == 0);
  // bool edgeLeft  = (leftIR == 0);


  // --------- Read Ultrasonic Distance ----------
  float distance = getDistanceCm();


  // --------- Debug Print ----------
  Serial.print("IR_Right=");
  Serial.print(rightIR);
  Serial.print(" EdgeR=");
  Serial.print(edgeRight);

  Serial.print(" | IR_Left=");
  Serial.print(leftIR);
  Serial.print(" EdgeL=");
  Serial.print(edgeLeft);

  Serial.print(" | Distance=");
  Serial.print(distance);
  Serial.println(" cm");


  // =================================================
  //            MAIN DECISION LOGIC
  // =================================================

  // 1️⃣ EDGE DETECTED → Most important (fall prevention)
  if (edgeLeft || edgeRight) {

    Serial.println(">> EDGE DETECTED! Backing away and turning.");

    stopMotors();
    delay(100);

    moveBackward();           // Move back from edge
    delay(backTimeEdge);

    // Turn away from the detected edge
    if (edgeLeft && !edgeRight) {
      turnRight();            // Left edge → turn right
    } 
    else if (edgeRight && !edgeLeft) {
      turnLeft();             // Right edge → turn left
    } 
    else {
      turnLeft();             // Both edges → turn left (safe default)
    }

    delay(turnTimeEdge);
    stopMotors();
  }


  // 2️⃣ OBSTACLE DETECTED → Ultrasonic
  else if (distance > 0 && distance < frontObstacleDist) {

    Serial.println(">> OBSTACLE AHEAD! Back + small turn.");

    stopMotors();
    delay(100);

    moveBackward();
    delay(backTimeObstacle);

    turnLeft();      // You can randomize later if needed
    delay(turnTimeObstacle);

    stopMotors();
  }


  // 3️⃣ SAFE → MOVE FORWARD
  else {
    Serial.println(">> CLEAR PATH → Moving Forward.");
    moveForward();
  }


  delay(40);  // Smooth loop delay
}



// =====================================================
//                 HELPER FUNCTIONS
// =====================================================

// ---- Ultrasonic distance measurement ----
float getDistanceCm() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1;               // No echo detected

  return (duration * 0.034) / 2.0;            // Convert to cm
}


// ---- Motor Functions ----

void moveForward() {
  // Right motor forward
  analogWrite(IN1, forwardSpeed);
  digitalWrite(IN2, LOW);

  // Left motor forward
  analogWrite(IN3, forwardSpeed);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  analogWrite(IN2, backSpeed);

  digitalWrite(IN3, LOW);
  analogWrite(IN4, backSpeed);
}

void turnLeft() {
  analogWrite(IN1, turnSpeed);  // Right motor forward
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);       // Left motor backward
  analogWrite(IN4, turnSpeed);
}

void turnRight() {
  digitalWrite(IN1, LOW);       // Right motor backward
  analogWrite(IN2, turnSpeed);

  analogWrite(IN3, turnSpeed);  // Left motor forward
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
