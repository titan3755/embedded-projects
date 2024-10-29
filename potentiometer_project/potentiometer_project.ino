// this code has been taken from an Arduino UNO example and modified for use with 10 segment bar displays

int sensorPin = A0;   // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
const int digPinStart = 2;
const int digPinEnd = 11;
int constVal = 1000;

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  for (int i = digPinStart; i <= digPinEnd; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  // stop the program for <sensorValue> milliseconds:
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  if (sensorValue > 0) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
  if (sensorValue >= 100) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }
  if (sensorValue >= 200) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }
  if (sensorValue >= 300) {
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(5, LOW);
  }
  if (sensorValue >= 400) {
    digitalWrite(6, HIGH);
  } else {
    digitalWrite(6, LOW);
  }
  if (sensorValue >= 500) {
    digitalWrite(7, HIGH);
  } else {
    digitalWrite(7, LOW);
  }
  if (sensorValue >= 600) {
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }
  if (sensorValue >= 700) {
    digitalWrite(9, HIGH);
  } else {
    digitalWrite(9, LOW);
  }
  if (sensorValue >= 800) {
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }
  if (sensorValue >= 900) {
    digitalWrite(11, HIGH);
  } else {
    digitalWrite(11, LOW);
  }
  if (sensorValue >= 1000) {
    digitalWrite(11, HIGH);
  } else {
    digitalWrite(11, LOW);
  }
  // stop the program for <sensorValue> milliseconds:
  Serial.print("values=");
  Serial.print(sensorValue);
  Serial.print("Value=");
  Serial.println(constVal);
}
