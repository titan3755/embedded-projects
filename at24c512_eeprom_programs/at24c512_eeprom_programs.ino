#include <Wire.h>
#define chipAddress 80
#define maxAddress 65536
#define minAddress 1
#define stAddr 1
#define times 60
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  char myStr[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut a nulla tellus. Sed id elit dui. Praesent posuere placerat arcu non porttitor. Nulla tincidunt diam elit, non placerat eros aliquet nec. In venenatis metus vitae nunc aliquet mollis sed ac arcu. Vivamus dictum ante eu faucibus vehicula. Pellentesque tempus ultricies nulla at ultrices.Curabitur ut cursus mauris. Aliquam eget cursus est, ut dignissim tellus. Proin tristiq";
  bigStringWriter(myStr, stAddr);
  bigStringReader(stAddr, strlen(myStr));
}

void loop() {

}

void bigStringWriter(char str[], unsigned int startAddr) {
  digitalWrite(13, LOW);
  unsigned int addrCount = startAddr;
  for (int j = 0; j < times; j++) {
    for (int i = 0; i < strlen(str); i++) {
      writeTo(chipAddress, addrCount, (int)str[i]);
      addrCount++;
    }
  }
}

void bigStringReader(unsigned int startAddr, size_t strLen) {
  digitalWrite(13, HIGH);
  Serial.print("Result str: ");
  for (int i = startAddr; i <= (startAddr + (times * strLen) - 1); i++) {
    Serial.print((char)readFrom(chipAddress, i));
  }
  Serial.print("\nEnd EEPROM Procedure");
}

void indvWriter(unsigned int addr, byte num) {
  writeTo(chipAddress, addr, num);
}

void addressReader(unsigned int addr) {
  Serial.print("Address Value: " + String(readFrom(chipAddress, addr)));
}

void nameWriterAndReader() {
  char name[18] = {'M', 'A', 'H', 'M', 'U', 'D', ' ', 'A', 'L', ' ', 'M', 'U', 'H', 'A', 'I', 'M', 'I', 'N'};
  for (int i = minAddress; i <= 18; i++) {
    writeTo(chipAddress, i, name[i - 1]);
  }
  for (int j = minAddress; j <= 18; j++) {
    Serial.print((char)readFrom(chipAddress, j));
  }
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
