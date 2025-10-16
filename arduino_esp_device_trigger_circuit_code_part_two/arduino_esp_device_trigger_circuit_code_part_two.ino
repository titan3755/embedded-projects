#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* WIFI_SSID = "ghughu";
const char* WIFI_PASSWORD = "ghughu01922";
const char* SECRET_PASSWORD = "admin";
const byte rxPin = 14;
const byte txPin = 12;
ESP8266WebServer server(80);
SoftwareSerial arduinoSerial(rxPin, txPin);
Adafruit_SSD1306 display(128, 64, &Wire, -1);
bool isLoggedIn = false;
unsigned long timerValue = 0;
String timerStatus = "Idle";
char receivedChars[32];
boolean newDataFromArduino = false;

String formatTime(unsigned long totalSeconds) {
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;
  char timeString[9];
  sprintf(timeString, "%02d:%02d:%02d", hours, minutes, seconds);
  return String(timeString);
}

String getHtmlPage(bool showControls) {
  String html = "<!DOCTYPE html><html><head><title>Timer Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:sans-serif;text-align:center;background:#282c34;color:white;}";
  html += "h1{color:#61dafb;} .card{background:#3c4049;padding:20px;border-radius:10px;max-width:300px;margin:auto;}";
  html += "input,button{width:80%;padding:10px;margin:5px;border-radius:5px;border:none;font-size:1em;} button{background:#61dafb;color:black;cursor:pointer;}";
  html += "button.reset{background:#e57373;} #timer{font-size:2.5em;margin:20px;color:#f9a825;}</style>";
  html += "<script>setInterval(function(){fetch('/getTime').then(response=>response.text()).then(data=>{document.getElementById('timer').innerText=data;});},1000);</script>";
  html += "</head><body><h1>ESP8266 Timer Control</h1><div class='card'>";
  if (showControls) {
    html += "<h2>Controls</h2><div id='timer'>" + formatTime(timerValue) + "</div>";
    if (timerStatus == "Finished") {
      html += "<p>Timer Finished!</p><form action='/reset' method='POST'><button class='reset' type='submit'>Reset Timer</button></form>";
    } else {
      html += "<form action='/start' method='POST'>";
      html += "<input type='number' name='seconds' placeholder='Total Seconds' required>";
      html += "<button type='submit'>Start Timer</button></form>";
    }
  } else {
    html += "<h2>Login</h2><form action='/login' method='POST'>";
    html += "<input type='password' name='password' placeholder='Password' required>";
    html += "<button type='submit'>Login</button></form>";
  }
  
  html += "</div></body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHtmlPage(isLoggedIn));
}

void handleLogin() {
  if (server.hasArg("password") && server.arg("password") == SECRET_PASSWORD) {
    isLoggedIn = true;
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  } else {
    handleRoot();
  }
}

void handleStart() {
  if (!isLoggedIn) { handleRoot(); return; }
  if (server.hasArg("seconds")) {
    String secondsStr = server.arg("seconds");
    arduinoSerial.println("START:" + secondsStr);
    timerStatus = "Running";
    timerValue = secondsStr.toInt();
  }
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleReset() {
  if (!isLoggedIn) { handleRoot(); return; }
  arduinoSerial.println("RESET");
  timerStatus = "Idle";
  timerValue = 0;
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleGetTime() {
  server.send(200, "text/plain", formatTime(timerValue));
}

void updateOled() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("IP: " + WiFi.localIP().toString());
  display.println("Status: " + timerStatus);
  display.setCursor(0, 32);
  display.setTextSize(2);
  display.println(formatTime(timerValue));
  display.display();
}

void receiveFromArduino() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    while (arduinoSerial.available() > 0 && newDataFromArduino == false) {
        rc = arduinoSerial.read();
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= sizeof(receivedChars)) { ndx = sizeof(receivedChars) - 1; }
        } else {
            receivedChars[ndx] = '\0';
            ndx = 0;
            newDataFromArduino = true;
        }
    }
}

void handleArduinoComm() {
  receiveFromArduino();
  if (newDataFromArduino) {
    String msg(receivedChars);
    if (msg.startsWith("TIME:")) {
      timerValue = msg.substring(5).toInt();
      if (timerValue == 0 && timerStatus != "Idle") {
        timerStatus = "Finished";
      } else if (timerValue > 0) {
        timerStatus = "Running";
      }
    }
    newDataFromArduino = false;
  }
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("Connecting to 'ghughu'...");
  display.display();
  arduinoSerial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }
  display.println("\nConnected!");
  display.println(WiFi.localIP().toString());
  display.display();
  delay(2000);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/start", HTTP_POST, handleStart);
  server.on("/getTime", HTTP_GET, handleGetTime);
  server.on("/reset", HTTP_POST, handleReset);
  server.begin();
}

void loop() {
  server.handleClient();
  handleArduinoComm();
  updateOled();
}