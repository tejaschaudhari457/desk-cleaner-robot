// ---------------- MOTOR PINS (L293D) ----------------
const int IN1 = 5;     // Right Motor Forward (PWM)
const int IN2 = 6;     // Right Motor Backward (PWM)
const int IN3 = 9;     // Left Motor Forward  (PWM)
const int IN4 = 10;    // Left Motor Backward (PWM)

// ---------------- ULTRASONIC PINS ----------------
#define TRIG 3
#define ECHO 4

// ---------------- IR SENSOR PINS ----------------
int irPinRight = 8;  
int irPinLeft  = 2;  

// Analog IR
const int irAnalogRightPin = A4;
const int irAnalogLeftPin  = A6;

// SOFTWARE INVERSION
bool invertMotors = true;

// ---------------- EMERGENCY TIMINGS ----------------
const unsigned long STOP_MS    = 150;
const unsigned long REVERSE_MS = 600;
const unsigned long TURN_MS    = 450;

// ---------------- THRESHOLDS ----------------
const long DISTANCE_EMERGENCY_CM = 5;
const int IR_EMERGENCY_THRESHOLD = 500;

// ---------------- CUSTOM MOTOR SPEEDS ----------------
// Forward
const int LEFT_MOTOR_SPEED  = 80;
const int RIGHT_MOTOR_SPEED = 110;

// Reverse
const int LEFT_REVERSE_SPEED  = 80;
const int RIGHT_REVERSE_SPEED = 110;

// Turn
const int LEFT_TURN_SPEED  = 80;
const int RIGHT_TURN_SPEED = 110;

// ---------------- HELPER ----------------
int clippedSpeed(int s) {
  if (s < 0) return 0;
  if (s > 255) return 255;
  return s;
}

// ---------------- SETUP ----------------
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

// ---------------- ULTRASONIC ----------------
long readUltrasonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

// ---------------- LOW LEVEL MOTOR ----------------
void rightForward(int speed)  { analogWrite(IN1, clippedSpeed(speed)); analogWrite(IN2, 0); }
void rightBackward(int speed) { analogWrite(IN1, 0); analogWrite(IN2, clippedSpeed(speed)); }
void leftForward(int speed)   { analogWrite(IN3, clippedSpeed(speed)); analogWrite(IN4, 0); }
void leftBackward(int speed)  { analogWrite(IN3, 0); analogWrite(IN4, clippedSpeed(speed)); }

// ---------------- LOGICAL MOVEMENTS ----------------
void logicalForward() {
  rightForward(RIGHT_MOTOR_SPEED);
  leftForward(LEFT_MOTOR_SPEED);
}

void logicalBackward() {
  rightBackward(RIGHT_REVERSE_SPEED);
  leftBackward(LEFT_REVERSE_SPEED);
}

void logicalTurnLeft() {
  rightForward(RIGHT_TURN_SPEED);
  leftBackward(LEFT_TURN_SPEED);
}

// ---------------- USER COMMANDS ----------------
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
    rightBackward(RIGHT_TURN_SPEED);
    leftForward(LEFT_TURN_SPEED);
  } else {
    logicalTurnLeft();
  }
}

void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// ---------------- EMERGENCY ----------------
void emergencyManeuver() {
  Serial.println("🚨 EMERGENCY");

  stopMotors();
  delay(STOP_MS);

  goBackward();
  delay(REVERSE_MS);

  stopMotors();
  delay(100);

  turnLeft();
  delay(TURN_MS);

  goForward();
}

// ---------------- LOOP ----------------
void loop() {
  int rightIR_analog = analogRead(irAnalogRightPin);
  int leftIR_analog  = analogRead(irAnalogLeftPin);
  long distance = readUltrasonic();

  Serial.print("IR_R:"); Serial.print(rightIR_analog);
  Serial.print(" IR_L:"); Serial.print(leftIR_analog);
  Serial.print(" Dist:");
  if (distance < 0) Serial.print("NA");
  else Serial.print(distance);
  Serial.println("cm");

  bool distanceEmergency = (distance > 0 && distance <= DISTANCE_EMERGENCY_CM);
  bool irEmergency = (rightIR_analog > IR_EMERGENCY_THRESHOLD ||
                      leftIR_analog  > IR_EMERGENCY_THRESHOLD);

  if (distanceEmergency || irEmergency) {
    emergencyManeuver();
  } else {
    goForward();
  }

  delay(120);
}
