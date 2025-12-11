// ---------------- MOTOR PINS (L293D) ----------------
int IN1 = 5;     // Right Motor Forward (logical)
int IN2 = 6;     // Right Motor Backward (logical)
int IN3 = 9;     // Left Motor Forward  (logical)
int IN4 = 10;    // Left Motor Backward (logical)

// ---------------- ULTRASONIC PINS ----------------
#define TRIG 3
#define ECHO 4

// ---------------- IR SENSOR PINS ----------------
int irPinRight = 8;  // Right IR (0 = surface, 1 = edge)
int irPinLeft  = 2;  // Left IR  (0 = surface, 1 = edge)

// SOFTWARE INVERSION (use when wiring is physically reversed)
bool invertMotors = true;  

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

// User-facing motor commands (respect invertMotors)
void goForward() {
  if (invertMotors) logicalBackward();
  else logicalForward();
}

void goBackward() {
  if (invertMotors) logicalForward();
  else logicalBackward();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  int rightIR = digitalRead(irPinRight);
  int leftIR  = digitalRead(irPinLeft);
  long distance = readUltrasonic();

  Serial.print("Right IR: ");
  Serial.print(rightIR);
  Serial.print(" | Left IR: ");
  Serial.print(leftIR);
  Serial.print(" | Ultrasonic: ");
  if (distance < 0) Serial.print("no echo");
  else {
    Serial.print(distance);
    Serial.print(" cm");
  }
  Serial.println();

  // Obstacle logic: distance <= 7 cm -> go backward, else forward
  if (distance > 0 && distance <= 7) {
    goBackward();
    Serial.println("Obstacle <= 7cm -> BACKWARD");
  } else {
    goForward();
    Serial.println("Clear -> FORWARD");
  }

  delay(100);
}
