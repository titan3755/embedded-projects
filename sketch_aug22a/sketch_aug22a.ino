const int BEEPER = 5;
const int SAFE_LED = 4;
const int ARMED_LED = 3;
const int DET_LED = 2;
const int BTN_ZERO = 9;
bool btn_zero_state = false;
const int BTN_ONE = 10;
bool btn_one_state = false;
const int BTN_PRESS_INDICATOR = 11;
char password[6] = "";
const char* correctPassword = "10101";
enum ProgramState { IDLE, ARMED, ENDED };
ProgramState currentState = IDLE;
const int DEBOUNCE_DELAY = 200;
const int latchPin = 6;
const int clockPin = 7;
const int dataPin = 8;

const byte digitPatterns[10] = {
  B11000000, // 0
  B11111001, // 1
  B10100100, // 2
  B10110000, // 3
  B10011001, // 4
  B10010010, // 5
  B10000010, // 6
  B11111000, // 7
  B10000000, // 8
  B10010000  // 9
};

int countdownSeconds = 30;
unsigned long previousMillis = 0;
const long interval = 1000; // 1 second

void setup() {
  Serial.begin(9600);
  pinMode(BEEPER, OUTPUT);
  pinMode(SAFE_LED, OUTPUT);
  pinMode(ARMED_LED, OUTPUT);
  pinMode(DET_LED, OUTPUT);
  pinMode(BTN_ZERO, INPUT);
  pinMode(BTN_ONE, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(BTN_PRESS_INDICATOR, OUTPUT);
  
  digitalWrite(SAFE_LED, HIGH);
  digitalWrite(ARMED_LED, LOW);
  digitalWrite(DET_LED, LOW);
  updateDisplays(countdownSeconds);
}

void loop() {
  switch (currentState) {
    case IDLE:
      handlePasswordInput();
      break;
    case ARMED:
      handleCountdown();
      break;
    case ENDED:
      break;
  }
}

void handlePasswordInput() {
  if (digitalRead(BTN_ZERO) == HIGH && btn_zero_state == false) {
    btn_zero_state = true;
    strcat(password, "0");
    Serial.println("zero");
    digitalWrite(BTN_PRESS_INDICATOR, HIGH);
    delay(DEBOUNCE_DELAY);
    digitalWrite(BTN_PRESS_INDICATOR, LOW);
  } else if (digitalRead(BTN_ZERO) == LOW && btn_zero_state == true) {
    btn_zero_state = false;
  }

  if (digitalRead(BTN_ONE) == HIGH && btn_one_state == false) {
    btn_one_state = true;
    strcat(password, "1");
    Serial.println("one");
    digitalWrite(BTN_PRESS_INDICATOR, HIGH);
    delay(DEBOUNCE_DELAY);
    digitalWrite(BTN_PRESS_INDICATOR, LOW);
  } else if (digitalRead(BTN_ONE) == LOW && btn_one_state == true) {
    btn_one_state = false;
  }

  if (strlen(password) >= 5) {
    if (strcmp(password, correctPassword) == 0) {
      digitalWrite(SAFE_LED, LOW);
      digitalWrite(ARMED_LED, HIGH);
      previousMillis = millis();
      currentState = ARMED;
    } else {
      password[0] = '\0';
    }
  }
}

void handleCountdown() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (countdownSeconds > 0) {
      countdownSeconds--;
      digitalWrite(BEEPER, HIGH);
      delay(50);
      digitalWrite(BEEPER, LOW);
    }
    
    updateDisplays(countdownSeconds);

    if (countdownSeconds <= 0) {
      digitalWrite(ARMED_LED, LOW);
      digitalWrite(DET_LED, HIGH);
      digitalWrite(BEEPER, HIGH);
      currentState = ENDED;
    }
  }
}

void updateDisplays(int number) {
  int tensDigit = number / 10;
  int onesDigit = number % 10;
  byte tensPattern = digitPatterns[tensDigit];
  byte onesPattern = digitPatterns[onesDigit];

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, onesPattern);
  shiftOut(dataPin, clockPin, MSBFIRST, tensPattern);
  digitalWrite(latchPin, HIGH);
}