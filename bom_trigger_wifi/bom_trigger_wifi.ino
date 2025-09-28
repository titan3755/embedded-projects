#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Wire.begin(D2, D1); // SDA = D3, SCL = D2
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  randomSeed(analogRead(A0)); // Seed randomness
}

void loop() {
  display.clearDisplay();

  int mode = random(0, 3);
  switch (mode) {
    case 0: // Random text
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(random(0, SCREEN_WIDTH - 50), random(0, SCREEN_HEIGHT - 10));
      display.print("Hello ");
      display.print(random(1000));
      break;

    case 1: // Random rectangle
      display.drawRect(random(0, SCREEN_WIDTH - 20), random(0, SCREEN_HEIGHT - 20),
                       random(10, 30), random(10, 30), SSD1306_WHITE);
      break;

    case 2: // Random circle
      display.drawCircle(random(10, SCREEN_WIDTH - 10), random(10, SCREEN_HEIGHT - 10),
                         random(5, 15), SSD1306_WHITE);
      break;
  }

  display.display();
  delay(1000);
}
