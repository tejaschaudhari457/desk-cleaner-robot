// ---------------- MOTOR PINS (L293D) ----------------
const int IN1 = 5;     // Right Motor Forward
const int IN2 = 6;     // Right Motor Backward
const int IN3 = 9;     // Left Motor Forward
const int IN4 = 10;    // Left Motor Backward

const int MOTOR_SPEED = 70;   // Testing speed

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // -------- FORWARD --------
  analogWrite(IN1, MOTOR_SPEED);   // Right forward
  analogWrite(IN2, 0);
  analogWrite(IN3, MOTOR_SPEED);   // Left forward
  analogWrite(IN4, 0);
  delay(1000);

  // -------- STOP --------
  stopMotors();
  delay(1000);

  // -------- BACKWARD --------
  analogWrite(IN1, 0);
  analogWrite(IN2, MOTOR_SPEED);   // Right backward
  analogWrite(IN3, 0);
  analogWrite(IN4, MOTOR_SPEED);   // Left backward
  delay(1000);

  // -------- STOP --------
  stopMotors();
  delay(1000);
}

void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
