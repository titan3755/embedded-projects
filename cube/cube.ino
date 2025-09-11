#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float vertices[8][3] = {
  {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
  {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
};

int edges[12][2] = {
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

float angleX = 0;
float angleY = 0;
float angleZ = 0;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;); 
  }
}

void loop() {
  display.clearDisplay();

  angleX += 0.02;
  angleY += 0.03;
  angleZ += 0.01;

  for (int i = 0; i < 12; i++) {
    float* v1 = vertices[edges[i][0]];
    float* v2 = vertices[edges[i][1]];
    float r1x = v1[2] * sin(angleY) + v1[0] * cos(angleY);
    float r1z = v1[2] * cos(angleY) - v1[0] * sin(angleY);
    float r1y = v1[1] * cos(angleX) - r1z * sin(angleX);
    float final_r1z = v1[1] * sin(angleX) + r1z * cos(angleX);
    float scale = 68; // Scale factor for size
    float distance = 4; // "Distance" of camera from cube
    float p1x = (SCREEN_WIDTH / 2) + (r1x * scale) / (final_r1z + distance);
    float p1y = (SCREEN_HEIGHT / 2) + (r1y * scale) / (final_r1z + distance);
    float r2x = v2[2] * sin(angleY) + v2[0] * cos(angleY);
    float r2z = v2[2] * cos(angleY) - v2[0] * sin(angleY);
    float r2y = v2[1] * cos(angleX) - r2z * sin(angleX);
    float final_r2z = v2[1] * sin(angleX) + r2z * cos(angleX);
    float p2x = (SCREEN_WIDTH / 2) + (r2x * scale) / (final_r2z + distance);
    float p2y = (SCREEN_HEIGHT / 2) + (r2y * scale) / (final_r2z + distance);
    display.drawLine(p1x, p1y, p2x, p2y, SSD1306_WHITE);
  }

  display.display();
}