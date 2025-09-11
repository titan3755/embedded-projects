#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 11, 12);
bool rhOk = false;
unsigned long lastRetry = 0;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println();
  Serial.println("DEBUG: booting (RH_ASK receiver)");
  Serial.println("DEBUG: Checking RH_ASK init...");

  rhOk = driver.init();
  Serial.print("DEBUG: driver.init() -> ");
  Serial.println(rhOk ? "OK" : "FAILED");

  if (rhOk) {
    Serial.println("433 MHz Receiver ready");
  } else {
    Serial.println("RH_ASK init failed. Will retry every 5s. Check wiring/library.");
    pinMode(LED_BUILTIN, OUTPUT);
  }
}

void loop() {
  if (!rhOk) {
    static unsigned long blinkT = 0;
    static bool ledState = false;
    if (millis() - blinkT > 500) {
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
      blinkT = millis();
    }
    if (millis() - lastRetry > 5000) {
      lastRetry = millis();
      Serial.println("DEBUG: retrying driver.init()");
      rhOk = driver.init();
      Serial.print("DEBUG: retry -> ");
      Serial.println(rhOk ? "OK" : "FAILED");
      if (rhOk) {
        Serial.println("433 MHz Receiver ready");
        digitalWrite(LED_BUILTIN, LOW); // stop blinking
      }
    }
    return;
  }

  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) {
    buf[buflen] = 0;
    Serial.print("Received (len=");
    Serial.print(buflen);
    Serial.print("): ");
    Serial.println((char*)buf);
  }
}
