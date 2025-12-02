// Motor 1 pins
int IN1 = 5;
int IN2 = 6;

// Motor 2 pins
int IN3 = 3;
int IN4 = 4;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {

  // ----- Motor 1 Forward -----
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // ----- Motor 2 Forward -----
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(2000);

  // ----- Motor 1 Reverse -----
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // ----- Motor 2 Reverse -----
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(2000);

  // ----- Stop both motors -----
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(2000);
}
