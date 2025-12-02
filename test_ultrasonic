#define TRIG 9
#define ECHO 10

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  // Clear the trigger
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  // Send trigger pulse (10us)
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Read echo pulse time (microseconds)
  long duration = pulseIn(ECHO, HIGH);

  // Convert echo time to distance (cm)
  float distance = duration * 0.034 / 2;

  // Print to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(300);
}
