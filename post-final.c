// ---------------- MOTOR PINS (L293D) ----------------
const int IN1 = 5;     // Right Motor Forward (PWM-capable)
const int IN2 = 6;     // Right Motor Backward (PWM-capable)
const int IN3 = 9;     // Left Motor Forward  (PWM-capable)
const int IN4 = 10;    // Left Motor Backward (PWM-capable)

// ---------------- ULTRASONIC PINS ----------------
#define TRIG 3
#define ECHO 4

// ---------------- IR SENSOR PINS ----------------
int irPinRight = 8;  // (digital) Right IR
int irPinLeft  = 2;  // (digital) Left IR

// Analog pins for analog IR sensors (used for threshold > 500)
const int irAnalogRightPin = A0;
const int irAnalogLeftPin  = A1;

// SOFTWARE INVERSION (use when wiring is physically reversed)
bool invertMotors = true;

// Emergency timing (milliseconds)
const unsigned long STOP_MS    = 150;
const unsigned long REVERSE_MS = 600;
const unsigned long TURN_MS    = 450;

// Thresholds
const long DISTANCE_EMERGENCY_CM = 5;
const int IR_EMERGENCY_THRESHOLD  = 500; // analogRead threshold (0..1023)

// ----- Motor speeds (0..255) - tweak to reduce/increase RPM -----
const int MOTOR_SPEED      = 70; // forward cruising speed (lower -> slower RPM)
const int REVERSE_SPEED    = 70; // reverse speed during emergency
const int TURN_SPEED       = 70; // speed used while turning (one motor forward, one back)

// Helper to constrain speeds safely (optional)
int clippedSpeed(int s){
  if(s < 0) return 0;
  if(s > 255) return 255;
  return s;
}

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

// Low-level PWM motor helpers
void rightForward(int speed) { analogWrite(IN1, clippedSpeed(speed)); analogWrite(IN2, 0); }
void rightBackward(int speed){ analogWrite(IN1, 0); analogWrite(IN2, clippedSpeed(speed)); }
void leftForward(int speed)  { analogWrite(IN3, clippedSpeed(speed)); analogWrite(IN4, 0); }
void leftBackward(int speed) { analogWrite(IN3, 0); analogWrite(IN4, clippedSpeed(speed)); }

// Logical motor commands (based on IN pins) using PWM speeds
void logicalForward(int speed) {
  rightForward(speed);
  leftForward(speed);
}

void logicalBackward(int speed) {
  rightBackward(speed);
  leftBackward(speed);
}

// Turn left: Right motor forward, Left motor backward
void logicalTurnLeft(int speed) {
  rightForward(speed);
  leftBackward(speed);
}

// User-facing motor commands (respect invertMotors)
void goForward() {
  if (invertMotors) logicalBackward(MOTOR_SPEED);
  else logicalForward(MOTOR_SPEED);
}

void goBackward() {
  if (invertMotors) logicalForward(REVERSE_SPEED);
  else logicalBackward(REVERSE_SPEED);
}

void turnLeft() {
  if (invertMotors) {
    // inverted: Right backward, Left forward
    rightBackward(TURN_SPEED);
    leftForward(TURN_SPEED);
  } else {
    logicalTurnLeft(TURN_SPEED);
  }
}

void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// Emergency maneuver: stop -> reverse -> turn left
void emergencyManeuver() {
  Serial.println("--- EMERGENCY START ---");
  stopMotors();
  delay(STOP_MS);

  // Reverse both motors (move backward)
  goBackward();
  Serial.println("Reversing...");
  delay(REVERSE_MS);

  stopMotors();
  delay(100);

  // Turn left
  turnLeft();
  Serial.println("Turning left...");
  delay(TURN_MS);

  // Resume forward after maneuver
  goForward();
  Serial.println("--- EMERGENCY END, RESUME FORWARD ---");
}

void loop() {
  int rightIR_digital = digitalRead(irPinRight);
  int leftIR_digital  = digitalRead(irPinLeft);

  int rightIR_analog = analogRead(irAnalogRightPin);
  int leftIR_analog  = analogRead(irAnalogLeftPin);

  long distance = readUltrasonic();

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

  bool distanceEmergency = (distance > 0 && distance <= DISTANCE_EMERGENCY_CM);
  bool rightIREmergency  = (rightIR_analog > IR_EMERGENCY_THRESHOLD);
  bool leftIREmergency   = (leftIR_analog  > IR_EMERGENCY_THRESHOLD);

  if (distanceEmergency || rightIREmergency || leftIREmergency) {
    Serial.print("EMERGENCY: ");
    if (distanceEmergency) Serial.print("Distance ");
    if (rightIREmergency) Serial.print("RightIR ");
    if (leftIREmergency)  Serial.print("LeftIR ");
    Serial.println();

    emergencyManeuver();
  }
  else {
    goForward();
    Serial.println("Safe -> FORWARD");
  }

  delay(120);
}
