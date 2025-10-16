#include <SoftwareSerial.h>

const int latchPin = 3, clockPin = 4, dataPin  = 2;
const int greenLedPin = 5, yellowLedPin = 6, redLedPin  = 7;
const int buzzerPin = 8;
const byte rxPin = 10, txPin = 11;
enum TimerState { IDLE, RUNNING, FINISHED };
TimerState currentState = IDLE;

SoftwareSerial espSerial(rxPin, txPin, true);

unsigned long timerValue = 0;
unsigned long lastSecondMillis = 0;
unsigned long lastSentTime = 1;
byte digit_map[] = { B11000000, B11111001, B10100100, B10110000, B10011001, B10010010, B10000010, B11111000, B10000000, B10010000 };

void updateHardwareState() {
  switch (currentState) {
    case IDLE:
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(yellowLedPin, LOW);
      digitalWrite(redLedPin, LOW);
      break;
    case RUNNING:
      digitalWrite(greenLedPin, LOW);
      digitalWrite(yellowLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
      break;
    case FINISHED:
      digitalWrite(greenLedPin, LOW);
      digitalWrite(yellowLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
      break;
  }
}

void displayTime(unsigned long totalSeconds) {
  int leftDigit = 0, rightDigit = 0;

  if (totalSeconds >= 3600) {
    leftDigit = totalSeconds / 3600;
    rightDigit = (totalSeconds % 3600) / 60 / 10;
  } else if (totalSeconds >= 60) {
    int minutes = totalSeconds / 60;
    leftDigit = minutes / 10;
    rightDigit = minutes % 10;
  } else {
    leftDigit = totalSeconds / 10;
    rightDigit = totalSeconds % 10;
  }

  if (leftDigit > 9) leftDigit = 9;
  if (rightDigit > 9) rightDigit = 9;

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit_map[rightDigit]);
  shiftOut(dataPin, clockPin, MSBFIRST, digit_map[leftDigit]);
  digitalWrite(latchPin, HIGH);
}

void handleCommunication() {
  if (espSerial.available()) {
    String command = espSerial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("START:")) {
      timerValue = command.substring(6).toInt();
      currentState = RUNNING;
      lastSecondMillis = millis();
      lastSentTime = timerValue + 1;
    } else if (command == "RESET") {
      timerValue = 0;
      currentState = IDLE;
    }
  }

  if (currentState == RUNNING && timerValue != lastSentTime) {
    espSerial.print("TIME:");
    espSerial.println(timerValue);
    lastSentTime = timerValue;
  }
}

void updateTimer() {
  if (currentState == RUNNING && (millis() - lastSecondMillis >= 1000)) {
    lastSecondMillis = millis();
    if (timerValue > 0) {
      tone(buzzerPin, 2000, 100); 
      timerValue--;
    } 
    if (timerValue == 0) {
      currentState = FINISHED;
      tone(buzzerPin, 1000, 500);
      espSerial.println("TIME:0");
    }
  }
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  espSerial.begin(9600);
  currentState = IDLE;
  updateHardwareState();
  displayTime(0);
}

void loop() {
  handleCommunication();
  updateTimer();
  updateHardwareState();
  displayTime(timerValue);
}