// ---------------- MOTOR PINS (L293D) ----------------
int IN1 = 5;     // Right Motor Forward (logical)
int IN2 = 6;     // Right Motor Backward (logical)
int IN3 = 9;     // Left Motor Forward  (logical)
int IN4 = 10;    // Left Motor Backward (logical)

// ---------------- ULTRASONIC PINS ----------------
#define TRIG 3
#define ECHO 4

// ---------------- IR SENSOR PINS ----------------
// Digital pins you previously used (kept for reference)
int irPinRight = 8;  // (digital) Right IR
int irPinLeft  = 2;  // (digital) Left IR

// Analog pins for analog IR sensors (used for threshold > 500)
const int irAnalogRightPin = A0;
const int irAnalogLeftPin  = A1;

// SOFTWARE INVERSION (use when wiring is physically reversed)
bool invertMotors = true;

// Emergency timing (milliseconds) — tune these as needed
const unsigned long STOP_MS    = 150;  // short stop before reversing
const unsigned long REVERSE_MS = 600;  // reverse duration
const unsigned long TURN_MS    = 450;  // left-turn duration

// Thresholds
const long DISTANCE_EMERGENCY_CM = 7;   // ultrasonic threshold (cm)
const int IR_EMERGENCY_THRESHOLD  = 500; // analogRead threshold (0..1023)

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(irPinRight, INPUT);
  pinMode(irPinLeft, INPUT);

  // analog pins do not need pinMode on AVR/Arduino; left for clarity
  // pinMode(irAnalogRightPin, INPUT);
  // pinMode(irAnalogLeftPin, INPUT);

  // ensure motors stopped at start
  stopMotors();
}

// Read ultrasonic distance in cm (returns -1 on timeout/no echo)
long readUltrasonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); // 30 ms timeout
  if (duration == 0) return -1;
  long distance = duration * 0.034 / 2; // cm
  return distance;
}

// Logical motor commands (based on IN pins)
void logicalForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void logicalBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Turn left: Right motor forward, Left motor backward
void logicalTurnLeft() {
  digitalWrite(IN1, HIGH);  // Right forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Left backward
  digitalWrite(IN4, HIGH);
}

// User-facing motor commands (respect invertMotors)
void goForward() {
  if (invertMotors) logicalBackward();
  else logicalForward();
}

void goBackward() {
  if (invertMotors) logicalForward();
  else logicalBackward();
}

void turnLeft() {
  if (invertMotors) {
    // invert logicalTurnLeft: swap forward/backward roles
    // original logicalTurnLeft = Right forward, Left backward
    // inverted: Right backward, Left forward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    logicalTurnLeft();
  }
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Emergency maneuver: stop -> reverse -> turn left
void emergencyManeuver() {
  Serial.println("--- EMERGENCY START ---");
  // 1) Stop briefly
  stopMotors();
  delay(STOP_MS);

  // 2) Reverse both motors (move backward)
  goBackward();
  Serial.println("Reversing...");
  delay(REVERSE_MS);

  // 3) Short stop before turning (optional)
  stopMotors();
  delay(100);

  // 4) Turn left (Right motor forward, Left motor backward)
  turnLeft();
  Serial.println("Turning left...");
  delay(TURN_MS);

  // 5) Resume forward after maneuver
  goForward();
  Serial.println("--- EMERGENCY END, RESUME FORWARD ---");
}

void loop() {
  // Read sensors
  int rightIR_digital = digitalRead(irPinRight);
  int leftIR_digital  = digitalRead(irPinLeft);

  int rightIR_analog = analogRead(irAnalogRightPin);
  int leftIR_analog  = analogRead(irAnalogLeftPin);

  long distance = readUltrasonic();

  // Print sensor values
  Serial.print("D_R:"); Serial.print(rightIR_digital);
  Serial.print(" D_L:"); Serial.print(leftIR_digital);
  Serial.print(" | A_R:"); Serial.print(rightIR_analog);
  Serial.print(" A_L:"); Serial.print(leftIR_analog);
  Serial.print(" | Ultrasound: ");
  if (distance < 0) Serial.print("no echo");
  else {
    Serial.print(distance);
    Serial.print("cm");
  }
  Serial.println();

  // Evaluate emergency condition:
  // (distance <= 7 cm) OR (right analog IR > 500) OR (left analog IR > 500)
  bool distanceEmergency = (distance > 0 && distance <= DISTANCE_EMERGENCY_CM);
  bool rightIREmergency  = (rightIR_analog > IR_EMERGENCY_THRESHOLD);
  bool leftIREmergency   = (leftIR_analog  > IR_EMERGENCY_THRESHOLD);

  if (distanceEmergency || rightIREmergency || leftIREmergency) {
    Serial.print("EMERGENCY: ");
    if (distanceEmergency) Serial.print("Distance ");
    if (rightIREmergency) { Serial.print("RightIR "); }
    if (leftIREmergency)  { Serial.print("LeftIR "); }
    Serial.println();

    // perform the emergency routine
    emergencyManeuver();
  }
  else {
    // Safe path: move forward normally
    goForward();
    Serial.println("Safe -> FORWARD");
  }

  // small loop delay
  delay(120);
}
