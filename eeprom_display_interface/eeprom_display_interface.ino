#include <Wire.h>
#define chipAddress 80
#define maxAddress 65536
#define minAddress 1

#define mode 0 // ( 1 for write ) ( 0 for read only )

int latchPin = 5;      // Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 6;      // Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 4;       // Data pin of 74HC595 is connected to Digital pin 4
int resetPin = 7;

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */

int counter = 1;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  randomSeed(analogRead(0));
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  delay(100);
  digitalWrite(resetPin, HIGH);
  if (mode == 1) {
    for (int i = minAddress; i <= 5000; i++) {
      int randomN = random(1, 256);
      writeTo(chipAddress, i, randomN);
    }
  }
  Serial.println("Get ready to start!");
  delay(5000);
}

void loop() 
{
  resetShiftRegister();
  int readDt = (int)readFrom(chipAddress, counter);
  Serial.println("Num: " + (String)readDt);
  updateShiftRegister(readDt);
  counter++;
  delay(100);
}

void updateShiftRegister(int updateNum)
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, updateNum);
   digitalWrite(latchPin, HIGH);
}

void resetShiftRegister() {
  digitalWrite(resetPin, LOW);
  delay(1);
  digitalWrite(resetPin, HIGH);
}

void writeTo(int chipAddr, unsigned int cellAddr, byte wrtData) {
  Wire.beginTransmission(chipAddr);
  Wire.write((int) (cellAddr >> 8));
  Wire.write((int) (cellAddr & 0xFF));
  Wire.write(wrtData);
  Wire.endTransmission();
  delay(10);
}

byte readFrom(int chipAddr, unsigned int cellAddr) {
  Wire.beginTransmission(chipAddr);
  Wire.write((int) (cellAddr >> 8));
  Wire.write((int) (cellAddr & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(chipAddr, 1);
  byte rdData = 0;
  if (Wire.available()) {
    rdData = Wire.read();
  }
  return rdData;
}