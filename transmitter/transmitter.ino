
const int laserPin = 3;
String message = "Hello! ";
int bitDelay = 500;

void setup() {
  pinMode(laserPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Transmitter ready.");
}

void loop() {
  digitalWrite(laserPin, HIGH);
  delay(bitDelay * 3);
  digitalWrite(laserPin, LOW);
  delay(bitDelay * 3);
  Serial.println("Start Signal Sent. Sending message...");
  for (int i = 0; i < message.length(); i++) {
    char characterToSend = message[i];
    Serial.print("Sending: ");
    Serial.print(characterToSend);
    Serial.print(" -> ");
    for (int j = 7; j >= 0; j--) {
      if (bitRead(characterToSend, j) == 1) {
        digitalWrite(laserPin, HIGH);
        Serial.print("1");
      } else {
        digitalWrite(laserPin, LOW);
        Serial.print("0");
      }
      delay(bitDelay);
    }
    Serial.println();
  }
  
  digitalWrite(laserPin, LOW);
  delay(5000);
}