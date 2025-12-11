// ---------------- MOTOR PINS (L293D) ----------------
int IN1 = 5;     // Right Motor Forward
int IN2 = 6;     // Right Motor Backward
int IN3 = 9;     // Left Motor Forward
int IN4 = 10;    // Left Motor Backward

// ---------------- ULTRASONIC PINS ----------------
#define TRIG 3
#define ECHO 4

// ---------------- IR SENSOR PINS ----------------
int irPinRight = 8;  // Right IR (0 = surface, 1 = edge)
int irPinLeft  = 2;  // Left IR (0 = surface, 1 = edge)


// =====================================================
// SETUP
// =====================================================
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


// =====================================================
// READ ULTRASONIC FUNCTION
// =====================================================
long readUltrasonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  long distance = duration * 0.034 / 2; // in cm
  return distance;
}


// =====================================================
// SIMPLE MOTOR TEST (forward → reverse → stop)
// =====================================================
void motorTest() {

  // Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1500);

  // Reverse
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1500);

  // Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(1500);
}


// =====================================================
// MAIN LOOP
// =====================================================
void loop() {

  // ---- Read Sensors ----
  int rightIR = digitalRead(irPinRight);
  int leftIR  = digitalRead(irPinLeft);
  long distance = readUltrasonic();

  // ---- Print Values to Serial ----
  Serial.print("Right IR: ");
  Serial.print(rightIR);

  Serial.print(" | Left IR: ");
  Serial.print(leftIR);

  Serial.print(" | Ultrasonic: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ---- Run Motor Test (for now) ----
  motorTest();

  delay(200);
}
