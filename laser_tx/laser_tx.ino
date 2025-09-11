const int laserPin = 3;

void setup() {
  pinMode(laserPin, OUTPUT);
}

void loop() {
  digitalWrite(laserPin, HIGH);
  delay(10); // Laser stays on for 2 seconds
  digitalWrite(laserPin, LOW);
  delay(10); // Laser stays off for 2 seconds
}