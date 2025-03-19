#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Define the I2C addresses for the two displays */
#define I2C_ADDRESS_1 0x3C // First display address
#define I2C_ADDRESS_2 0x3D // Second display address

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin (or -1 if sharing Arduino reset pin)

/* Create two display objects */
Adafruit_SH1106G display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SH1106G display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Variables for 3D rotation */
float angle1 = 0; // Rotation angle for display1
float angle2 = 0; // Rotation angle for display2

void setup() {
  Serial.begin(9600);

  // Initialize the first display
  if (!display1.begin(I2C_ADDRESS_1, true)) {
    Serial.println(F("SSD1306 allocation failed for display1"));
    for (;;); // Halt if initialization fails
  }

  // Initialize the second display
  if (!display2.begin(I2C_ADDRESS_2, true)) {
    Serial.println(F("SSD1306 allocation failed for display2"));
    for (;;); // Halt if initialization fails
  }

  // Clear the buffers
  display1.clearDisplay();
  display2.clearDisplay();

  // Set text properties (optional)
  display1.setTextSize(1);
  display1.setTextColor(SH110X_WHITE);
  display2.setTextSize(1);
  display2.setTextColor(SH110X_WHITE);
}

void loop() {
  // Clear both displays
  display1.clearDisplay();
  display2.clearDisplay();

  // Draw a rotating cube on display1
  drawRotatingCube(&display1, angle1);
  angle1 += 0.05; // Increment rotation angle for display1

  // Draw a rotating pyramid on display2
  drawRotatingPyramid(&display2, angle2);
  angle2 += 0.03; // Increment rotation angle for display2

  // Update both displays
  display1.display();
  display2.display();

  // Small delay to control rotation speed
  delay(50);
}

/* Function to draw a rotating cube */
void drawRotatingCube(Adafruit_SH1106G *display, float angle) {
  // Define cube vertices (scaled down by 50%)
  float vertices[8][3] = {
    { -0.5, -0.5, -0.5 }, { 0.5, -0.5, -0.5 }, { 0.5, 0.5, -0.5 }, { -0.5, 0.5, -0.5 },
    { -0.5, -0.5, 0.5 }, { 0.5, -0.5, 0.5 }, { 0.5, 0.5, 0.5 }, { -0.5, 0.5, 0.5 }
  };

  // Define cube edges
  int edges[12][2] = {
    { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
    { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
    { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
  };

  // Project 3D points to 2D
  for (int i = 0; i < 8; i++) {
    float x = vertices[i][0];
    float y = vertices[i][1];
    float z = vertices[i][2];

    // Rotate around Y axis
    float xRot = x * cos(angle) - z * sin(angle);
    float zRot = x * sin(angle) + z * cos(angle);

    // Perspective projection (scaled down further)
    float scale = 100 / (zRot + 5); // Adjust scale for perspective
    int x2D = SCREEN_WIDTH / 2 + xRot * scale;
    int y2D = SCREEN_HEIGHT / 2 - y * scale;

    // Store projected points
    vertices[i][0] = x2D;
    vertices[i][1] = y2D;
  }

  // Draw edges
  for (int i = 0; i < 12; i++) {
    int start = edges[i][0];
    int end = edges[i][1];
    display->drawLine(vertices[start][0], vertices[start][1], vertices[end][0], vertices[end][1], SH110X_WHITE);
  }
}

/* Function to draw a rotating pyramid */
void drawRotatingPyramid(Adafruit_SH1106G *display, float angle) {
  // Define pyramid vertices (scaled down by 50%)
  float vertices[5][3] = {
    { 0, -0.5, 0 }, { -0.5, 0.5, -0.5 }, { 0.5, 0.5, -0.5 }, { 0.5, 0.5, 0.5 }, { -0.5, 0.5, 0.5 }
  };

  // Define pyramid edges
  int edges[8][2] = {
    { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 },
    { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 1 }
  };

  // Project 3D points to 2D
  for (int i = 0; i < 5; i++) {
    float x = vertices[i][0];
    float y = vertices[i][1];
    float z = vertices[i][2];

    // Rotate around Y axis
    float xRot = x * cos(angle) - z * sin(angle);
    float zRot = x * sin(angle) + z * cos(angle);

    // Perspective projection (scaled down further)
    float scale = 100 / (zRot + 5); // Adjust scale for perspective
    int x2D = SCREEN_WIDTH / 2 + xRot * scale;
    int y2D = SCREEN_HEIGHT / 2 - y * scale;

    // Store projected points
    vertices[i][0] = x2D;
    vertices[i][1] = y2D;
  }

  // Draw edges
  for (int i = 0; i < 8; i++) {
    int start = edges[i][0];
    int end = edges[i][1];
    display->drawLine(vertices[start][0], vertices[start][1], vertices[end][0], vertices[end][1], SH110X_WHITE);
  }
}
