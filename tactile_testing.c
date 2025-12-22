#define BUTTON_PIN 18

int counter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Button counter started");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    counter++;
    Serial.print("Counter = ");
    Serial.println(counter);

    // wait until button is released
    while (digitalRead(BUTTON_PIN) == LOW);
    delay(50); // small debounce
  }
}
