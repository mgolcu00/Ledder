#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
// NeoMatrix credentials
#define PIN 15        // Data pin
#define NUMPIXELS 256 // 32x8 matrix
#define BRIGHTNESS 80
#define MATRIX_TYPE NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG
#define MATRIX_COLOR NEO_GRB + NEO_KHZ800
#define SCREEN_INIT_TEXT "IDLE"

const int init_color[3] = {0, 0, 255}; // blue

const int init_x = 0;
const int init_y = 0;

// define pixel matrix
extern Adafruit_NeoMatrix matrix;

void initMatrix();
void setText(String text, int *color, void (*callback)(String));
void setBrightness(int brightness);
void moveCursor(String, int);
void setTextColor(int r, int g, int b);
#endif