#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128    // OLED width
#define SCREEN_HEIGHT 64    // OLED height

// Define the GPIO pins using their numbers for clarity
#define OLED_SDA_PIN D2     // GPIO 4
#define OLED_SCL_PIN D1     // GPIO 5
#define ARDUINO_INPUT_PIN 14 // D5 is GPIO 14 (Connected to the voltage divider)
#define ESP_LED_PIN 12      // D6 is GPIO 12 (Using a different pin for the ESP's output LED)

// Create display object connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize I2C
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);  // SDA = D2, SCL = D1
  Serial.begin(115200);

  // 1. Initialize the Arduino Input Pin (D5/GPIO 14)
  pinMode(ARDUINO_INPUT_PIN, INPUT); 

  // 2. Initialize the ESP Output LED Pin (D6/GPIO 12)
  pinMode(ESP_LED_PIN, OUTPUT); 

  // Initialize the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Initial OLED Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Hello!");
  display.setTextSize(1);
  display.println("Waiting for Nano...");
  display.display();
}

void loop() {
  // Read the digital state of the pin connected to the Arduino
  int arduinoState = digitalRead(ARDUINO_INPUT_PIN);

  // Control the ESP's LED based on the Arduino's signal
  if (arduinoState == HIGH) {
    // If Arduino sends HIGH (3.3V after divider), turn ESP LED ON
    digitalWrite(ESP_LED_PIN, HIGH);
    
    // Update OLED to show the state
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.println("SIGNAL: HIGH");
    display.setTextSize(1);
    display.println("ESP LED IS ON");
    display.display();
    
  } else {
    // If Arduino sends LOW (0V), turn ESP LED OFF
    digitalWrite(ESP_LED_PIN, LOW);
    
    // Update OLED to show the state
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(2);
    display.println("SIGNAL: LOW");
    display.setTextSize(1);
    display.println("ESP LED IS OFF");
    display.display();
  }
  
  // A small delay to keep the loop from running too fast
  delay(500); 
}