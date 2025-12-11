// ============= UPDATED PIN VERSION (LOGIC NOT CHANGED) =============

// ---------------- MOTOR PINS (L293D DRIVER) ----------------
int IN1 = 5;     // Right Motor Forward (PWM)
int IN2 = 6;     // Right Motor Backward (PWM)
int IN3 = 9;     // Left Motor Forward  (PWM)
int IN4 = 10;    // Left Motor Backward (PWM)


// ---------------- IR EDGE SENSOR PINS ----------------
int irPinRight = 8;   // Right side IR  (0 = surface, 1 = edge)
int irPinLeft  = 2;   // Left side IR   (0 = surface, 1 = edge)


// ---------------- ULTRASONIC (FRONT) -----------------
#define TRIG 3
#define ECHO 4


// ------------------------------------------------------
long duration;
int distance;
int distancecm;
int rightIRValue;
int leftIRValue;


// ------------------------------------------------------
// Ultrasonic distance function (SAME LOGIC)
int mdistance()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}


// ------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // IR pins
  pinMode(irPinRight, INPUT);
  pinMode(irPinLeft, INPUT);

  // Ultrasonic
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}


// ------------------------------------------------------
void loop() {

  distancecm = mdistance();                 // SAME
  rightIRValue = digitalRead(irPinRight);   // SAME
  leftIRValue  = digitalRead(irPinLeft);    // SAME

  // SAME DECISION LOGIC AS ORIGINAL CODE
  if (distancecm < 5 || rightIRValue == 1 || leftIRValue == 1) {

    // stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);


    // backward (same behavior)
    analogWrite(IN1, LOW);
    analogWrite(IN2, 180);   // Right backward
    analogWrite(IN3, LOW);
    analogWrite(IN4, 180);   // Left backward

    delay(1000);


    // turn left (same behavior)
    analogWrite(IN1, 180);   // Right forward
    analogWrite(IN2, LOW);

    analogWrite(IN3, LOW);
    analogWrite(IN4, 180);   // Left backward

    delay(1000);
  }
  else {
    // forward (same behavior)
    analogWrite(IN1, 180);   // Right forward
    analogWrite(IN2, LOW);

    analogWrite(IN3, 180);   // Left forward
    analogWrite(IN4, LOW);
  }

}
