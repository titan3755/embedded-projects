#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "ghughu";
const char* password = "ghughu01922";

ESP8266WebServer server(80);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String oledText = "Waiting for text...";
uint8_t oledTextSize = 1;

void updateOLED() {
display.clearDisplay();
display.setTextSize(oledTextSize);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 0);
display.setTextWrap(true);
display.println(oledText);
display.display();
}

void handleRoot() {
String html = "<!DOCTYPE html><html><head><title>OLED Text Updater</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><style>html { font-family: Helvetica, Arial, sans-serif; display: inline-block; text-align: center; }body { margin-top: 50px; }h1 { color: #444444; margin: 50px auto 30px; }.form-group { margin-bottom: 20px; }input[type=text] { width: 80%; padding: 12px 20px; margin: 8px 0; box-sizing: border-box; border: 2px solid #ccc; border-radius: 4px; }input[type=number] { width: 80%; padding: 12px 20px; margin: 8px 0; box-sizing: border-box; border: 2px solid #ccc; border-radius: 4px; }input[type=submit] { background-color: #4CAF50; color: white; padding: 14px 20px; margin: 8px 0; border: none; border-radius: 4px; cursor: pointer; width: 80%; }input[type=submit]:hover { background-color: #45a049; }</style></head><body><h1>ESP8266 OLED Control</h1><form action=\"/update\" method=\"get\"><div class=\"form-group\"><input type=\"text\" name=\"text\" placeholder=\"Enter text to display...\" value=\"" + oledText + "\"></div><div class=\"form-group\"><input type=\"number\" name=\"size\" placeholder=\"Text Size\" value=\"" + String(oledTextSize) + "\"></div><div class=\"form-group\"><input type=\"submit\" value=\"Update OLED\"></div></form></body></html>";
  server.send(200, "text/html", html);
}

void handleUpdate() {
if (server.hasArg("text")) {
if (server.arg("text") != "") {
oledText = server.arg("text");
Serial.print("Text received: ");
Serial.println(oledText);
}
}
if (server.hasArg("size")) {
if (server.arg("size") != "") {
oledTextSize = (uint8_t)atoi(server.arg("size").c_str());
Serial.print("Text Size Received: ");
Serial.println(oledTextSize);
}
}
updateOLED();
server.sendHeader("Location", "/");
server.send(303);
}

void handleNotFound() {
server.send(404, "text/plain", "404: Not found");
}

void setup() {
Serial.begin(115200);
delay(100);

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
for(;;);
}
Serial.println("OLED initialized.");
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("OLED Ready!");
display.display();
delay(1000);

Serial.print("Connecting to ");
Serial.println(ssid);
display.clearDisplay();
display.setCursor(0,0);
display.print("Connecting to WiFi");
display.display();

WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
display.print(".");
display.display();
}
Serial.println("\nWiFi connected!");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());

display.clearDisplay();
display.setCursor(0,0);
display.println("Connected!");
display.println("IP Address:");
display.println(WiFi.localIP());
display.display();

server.on("/", HTTP_GET, handleRoot);
server.on("/update", HTTP_GET, handleUpdate);
server.onNotFound(handleNotFound);

server.begin();
Serial.println("HTTP server started");
}

void loop() {
server.handleClient();
}