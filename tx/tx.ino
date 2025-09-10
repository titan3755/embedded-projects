#include <RH_ASK.h>
#include <SPI.h>

// Only TX (D12). RX pin is defined but unused.
RH_ASK driver(2000, 11, 12);  // (bitrate, rxPin, txPin)

void setup() {
  Serial.begin(9600);
  delay(500);

  if (!driver.init()) {
    Serial.println("RH_ASK init failed!");
    while (1);
  }
  Serial.println("433 MHz Transmitter ready");
}

void loop() {
  const char *msg = "Hello433";   // message to send
  driver.send((uint8_t*)msg, strlen(msg));
  driver.waitPacketSent();

  Serial.print("Sent: ");
  Serial.println(msg);

  delay(1000); // send once per second
}
