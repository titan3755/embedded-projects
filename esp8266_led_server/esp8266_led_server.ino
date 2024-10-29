#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ghughu";
const char* password = "ghughu01922";

ESP8266WebServer server(80);

uint8_t LED_ONBOARD = LED_BUILTIN;
bool LED_ONBOARD_STATUS = LOW;

void setup() {
  Serial.begin(230400);
  delay(100);
  pinMode(LED_ONBOARD, OUTPUT);
  Serial.print("ConnectingTo=");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.on("/onboardledon", handle_ledon);
  server.on("/onboardledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  if (LED_ONBOARD_STATUS) {
    digitalWrite(LED_ONBOARD, HIGH);
  } else {
    digitalWrite(LED_ONBOARD, LOW);
  }
}

void handle_OnConnect() {
  LED_ONBOARD_STATUS = LOW;
  Serial.println("BUILDIN LED STATUS = OFF");
  server.send(200, "text/html", SendHTML(LED_ONBOARD_STATUS));
}

void handle_ledon() {
  LED_ONBOARD_STATUS = HIGH;
  Serial.println("BUILDIN LED STATUS = ON");
  server.send(200, "text/html", SendHTML(LED_ONBOARD_STATUS));
}

void handle_ledoff() {
  LED_ONBOARD_STATUS = LOW;
  Serial.println("BUILDIN LED STATUS = OFF");
  server.send(200, "text/html", SendHTML(LED_ONBOARD_STATUS));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t LED_ONBOARD_STATUS){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>BUILDIN LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
  if(LED_ONBOARD_STATUS)
  {ptr +="<p>LED Status: ON</p><a class=\"button button-off\" href=\"/onboardledoff\">OFF</a>\n";}
  else
  {ptr +="<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/onboardledon\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}