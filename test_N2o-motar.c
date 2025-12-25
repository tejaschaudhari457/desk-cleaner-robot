// ===== MOTOR TEST CODE (FORWARD & BACKWARD ONLY) =====

// Motor pins
const int IN1 = 5;   // Right Motor Forward
const int IN2 = 6;   // Right Motor Backward
const int IN3 = 9;   // Left Motor Forward
const int IN4 = 10;  // Left Motor Backward

const int SPEED = 120;   // Test speed (0–255)

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
}

void loop() {
  // FORWARD
  forward();
  delay(3000);

  stopMotors();
  delay(1500);

  // BACKWARD
  backward();
  delay(3000);

  stopMotors();
  delay(2000);
}

// -------- Motor Control --------
void forward() {
  analogWrite(IN1, SPEED);
  analogWrite(IN2, 0);
  analogWrite(IN3, SPEED);
  analogWrite(IN4, 0);
}

void backward() {
  analogWrite(IN1, 0);
  analogWrite(IN2, SPEED);
  analogWrite(IN3, 0);
  analogWrite(IN4, SPEED);
}

void stopMotors() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
