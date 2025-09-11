#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int ldrPin = A0;
String receivedMessage = "";
int bitDelay = 100;
int lightThreshold = 325;

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Receiver Ready");
  display.println("Listening for start...");
  display.display();
  Serial.println("LDR_Value,Threshold,Decoded_Bit");
}

void loop() {
  if (analogRead(ldrPin) > lightThreshold) {
    delay(bitDelay * 2.5); 
    if (analogRead(ldrPin) < lightThreshold) {
      decodeMessage();
    }
  }
}

void decodeMessage() {
  receivedMessage = "";
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Receiving...");
  display.display();

  for (int i = 0; i < 10; i++) {
    byte receivedChar = 0;
    for (int j = 7; j >= 0; j--) {
      delay(bitDelay / 2);
      int currentLdrValue = analogRead(ldrPin);
      Serial.print(currentLdrValue);
      Serial.print(",");
      Serial.print(lightThreshold);
      Serial.print(",");
      if (currentLdrValue > lightThreshold) {
        bitSet(receivedChar, j);
        Serial.println(1023);
      } else {
        Serial.println(0);
      }
      delay(bitDelay / 2); 
    }
    receivedMessage += (char)receivedChar;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Message:");
    display.setCursor(0, 10);
    display.println(receivedMessage);
    display.display();
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Final Message:");
  display.setCursor(0, 10);
  display.println(receivedMessage);
  display.display();
  delay(3000); 
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Receiver Ready");
  display.println("Listening for start...");
  display.display();
}