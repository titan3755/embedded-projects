#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int motionSensorPin = 2;

int currentMotionState = LOW;
int lastMotionState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(motionSensorPin, INPUT);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Motion Sensor");
  display.println("Initializing...");
  display.display();
  Serial.println("Waiting for sensor to settle...");
  delay(30000); 
  Serial.println("Sensor ready!");
}

void loop() {
  currentMotionState = digitalRead(motionSensorPin);
  if (currentMotionState != lastMotionState) {
    display.clearDisplay();
    display.setTextSize(2); // Use a larger font
    display.setCursor(15, 25); // Center the text

    if (currentMotionState == HIGH) {
      Serial.println("Motion Detected!");
      display.println("DETECTED!");
    } 
    else {
      Serial.println("Status: Clear");
      display.println("CLEAR");
    }
    
    display.display();
    lastMotionState = currentMotionState;
  }
}