#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Servo settings
Servo myServo;
int servoPin = D6; // Changed to GPIO12
int angle = 0;     // Default angle

// WiFi credentials
const char* ssid = "ghughu_iot";
const char* password = "ghughu01922";

// Web server
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

  // Initialize OLED
  if (!display.begin(0x3C, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();

  // Initialize servo
  myServo.attach(servoPin, 500, 2500);
  myServo.write(angle); // Set to initial angle

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/set", handleSetAngle);
  server.begin();
  Serial.println("HTTP server started");

  // Display initial angle
  updateDisplay(angle);
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><head><title>Servo Control</title></head>"
                "<body>"
                "<h1>Control Servo Angle</h1>"
                "<input type='range' min='0' max='180' value='" + String(angle) + "' id='angleSlider' oninput='updateAngle(this.value)'>"
                "<p>Angle: <span id='angleValue'>" + String(angle) + "</span>°</p>"
                "<script>"
                "function updateAngle(value) {"
                "  document.getElementById('angleValue').innerText = value;"
                "  var xhttp = new XMLHttpRequest();"
                "  xhttp.open('GET', '/set?angle=' + value, true);"
                "  xhttp.send();"
                "}"
                "</script>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleSetAngle() {
  if (server.hasArg("angle")) {
    int newAngle = server.arg("angle").toInt();
    if (newAngle >= 0 && newAngle <= 180) {
      angle = newAngle;
      myServo.write(angle);
      updateDisplay(angle);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Invalid angle");
    }
  } else {
    server.send(400, "text/plain", "No angle provided");
  }
}

void updateDisplay(int angle) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Angle:");
  display.setCursor(0, 30);
  display.print(angle);
  display.display();
}
