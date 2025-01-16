#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Replace with your Wi-Fi credentials
const char* ssid = "ghughu_iot";
const char* password = "ghughu01922";

// OpenWeatherMap API settings
const char* apiKey = "0a828b1755ea9eccaef2aa6c52c745b5";
const char* city = "Dhaka"; // Replace with your city
const char* apiEndpoint = "http://api.openweathermap.org/data/2.5/weather?q=Dhaka&units=metric&appid=0a828b1755ea9eccaef2aa6c52c745b5";

// List of locations
const char* locations[] = {
  "London",
  "Beijing",
  "Dhaka",
  "Mumbai",
  "Singapore",
  "Tokyo",
  "Texas",
  "California",
  "Los Angeles",
  "Shanghai",
  "Delhi",
  "Sao Paulo",
  "Mexico City",
  "Cairo",
  "Jeddah",
  "Qatar",
  "Kuwait",
  "Kabul",
  "Moscow",
  "Sweden",
  "Denmark",
  "Iceland",
  "Greenland",
  "Ottawa",
  "Kathmandu",
  "Hanoi",
  "Barcelona",
  "Madrid",
  "Tuscany",
  "Rome",
  "Paris",
  "Istanbul"
};

const int locationCount = sizeof(locations) / sizeof(locations[0]);
int currentLocationIndex = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(0x3C, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  display.clearDisplay();

  WiFi.begin(ssid, password);
  displayMessage("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  displayMessage("Wi-Fi Connected!");
}

void loop() {
// Fetch weather data for the current location
  fetchWeather(locations[currentLocationIndex]);

  // Cycle through locations every 5 seconds
  delay(3000);
  currentLocationIndex = (currentLocationIndex + 1) % locationCount;
}

void displayMessage(const char* message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}

void fetchWeather(const char* city) {
  WiFiClient client;
  String apiEndpoint = String("http://api.openweathermap.org/data/2.5/weather?q=") + city +
                       "&units=metric&appid=" + apiKey;

  if (client.connect("api.openweathermap.org", 80)) {
    client.print(String("GET ") + apiEndpoint + " HTTP/1.1\r\n" +
                 "Host: api.openweathermap.org\r\n" +
                 "Connection: close\r\n\r\n");
    delay(1000);

    String line, payload;
    while (client.available()) {
      line = client.readStringUntil('\n');
      if (line == "\r") break; // End of headers
    }

    while (client.available()) {
      payload += client.readString();
    }

    // Parse JSON
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    String weather = doc["weather"][0]["main"];
    String temp = doc["main"]["temp"];

    // Display weather
    display.clearDisplay();
    drawWeatherIcon(weather);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println(city); // Display location
    display.println(weather); // Display weather description
    display.print("Temp: ");
    display.print(temp);
    display.println(" C");
    display.display();
  } else {
    displayMessage("Failed to connect!");
  }
}

void drawWeatherIcon(String weather) {
  if (weather == "Haze" || weather == "Clouds") {
    // Draw a cloud
    display.fillCircle(20, 20, 10, SSD1306_WHITE);
    display.fillCircle(30, 20, 8, SSD1306_WHITE);
  } else if (weather == "Clear") {
    // Draw a sun
    display.drawCircle(20, 20, 10, SSD1306_WHITE);
    for (int i = 0; i < 360; i += 45) {
      int x1 = 20 + 12 * cos(i * PI / 180);
      int y1 = 20 + 12 * sin(i * PI / 180);
      int x2 = 20 + 16 * cos(i * PI / 180);
      int y2 = 20 + 16 * sin(i * PI / 180);
      display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
    }
  } else if (weather == "Rain" || weather == "Drizzle") {
    // Draw raindrops
    for (int i = 0; i < 3; i++) {
      display.fillCircle(15 + i * 10, 20, 3, SSD1306_WHITE);
    }
  } else if (weather == "Snow") {
    // Draw snowflakes
    display.drawLine(15, 15, 25, 25, SSD1306_WHITE);
    display.drawLine(25, 15, 15, 25, SSD1306_WHITE);
    display.drawLine(20, 10, 20, 30, SSD1306_WHITE);
    display.drawLine(10, 20, 30, 20, SSD1306_WHITE);
  } else {
    // Default: Unknown weather (draw a question mark)
    display.setTextSize(2);
    display.setCursor(10, 10);
    display.print("?");
  }
}
