#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define the pin for the motion sensor
const int motionSensorPin = 2;

// Variable to store the current and previous state of the sensor
int currentMotionState = LOW;
int lastMotionState = LOW;

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize the motion sensor pin as an input
  pinMode(motionSensorPin, INPUT);

  // Initialize the OLED display with the I2C address 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the display buffer and show initial message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Motion Sensor");
  display.println("Initializing...");
  display.display();
  
  // PIR sensors need a minute to calibrate when first powered on
  Serial.println("Waiting for sensor to settle...");
  delay(30000); 
  Serial.println("Sensor ready!");
}

void loop() {
  // Read the state of the motion sensor
  currentMotionState = digitalRead(motionSensorPin);

  // Check if the state has changed since the last loop
  if (currentMotionState != lastMotionState) {
    display.clearDisplay();
    display.setTextSize(2); // Use a larger font
    display.setCursor(15, 25); // Center the text

    // If motion is detected
    if (currentMotionState == HIGH) {
      Serial.println("Motion Detected!");
      display.println("DETECTED!");
    } 
    // If no motion is detected
    else {
      Serial.println("Status: Clear");
      display.println("CLEAR");
    }
    
    // Update the display with the new information
    display.display();
    
    // Update the last known state
    lastMotionState = currentMotionState;
  }
}