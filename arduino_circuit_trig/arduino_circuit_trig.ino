const int LATCH_PIN = 3;
const int CLOCK_PIN = 4;
const int DATA_PIN = 2;
const int GREEN_LED = 5;
const int YELLOW_LED = 6;
const int RED_LED = 7;
const int BEEPER = 8;


/*
 * Segment-to-Bit Mapping:
 *
 * Bit:   7  6  5  4  3  2  1  0
 * Seg:   DP G  F  E  D  C  B  A
 *
 * For COMMON ANODE, a 0 turns the segment ON and a 1 turns it OFF.
 * The patterns below are "inverted".
 */
byte numberPatterns[10] = {
  B11000000,  // 0 : Segments A,B,C,D,E,F are ON (0)
  B11111001,  // 1 : Segments B,C are ON (0)
  B10100100,  // 2 : Segments A,B,G,E,D are ON (0)
  B10110000,  // 3 : Segments A,B,G,C,D are ON (0)
  B10011001,  // 4 : Segments F,G,B,C are ON (0)
  B10010010,  // 5 : Segments A,F,G,C,D are ON (0)
  B10000010,  // 6 : Segments A,F,E,D,C,G are ON (0)
  B11111000,  // 7 : Segments A,B,C are ON (0)
  B10000000,  // 8 : All segments are ON (0)
  B10010000   // 9 : Segments G,F,A,B,C,D are ON (0)
};

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i <= 99; i++) {
    updateDisplays(i);
    delay(500);
  }
}

void updateDisplays(int number) {
  int tensDigit = number / 10;
  int onesDigit = number % 10;

  byte tensPattern = numberPatterns[tensDigit];
  byte onesPattern = numberPatterns[onesDigit];
  digitalWrite(LATCH_PIN, LOW);

  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, onesPattern);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, tensPattern);

  digitalWrite(LATCH_PIN, HIGH);
}