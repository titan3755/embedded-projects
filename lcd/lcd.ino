#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));
}

void loop() {
  int col = random(0, 16);
  char c = char(random(33, 127));
  lcd.setCursor(col, 0);
  lcd.print(c);
  lcd.setCursor(col, 1);
  lcd.print(c);
  delay(random(50, 200));
  if (random(0, 10) > 7) {
    lcd.setCursor(col, 0); lcd.print(" ");
    lcd.setCursor(col, 1); lcd.print(" ");
  }
}
