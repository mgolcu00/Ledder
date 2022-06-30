
#include "screen.h"

Adafruit_NeoMatrix matrix =
    Adafruit_NeoMatrix(
        32,
        8,
        PIN,
        MATRIX_TYPE,
        MATRIX_COLOR);
        
void initMatrix()
{
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(BRIGHTNESS);
    matrix.setTextColor(matrix.Color(init_color[0], init_color[1], init_color[2]));
    matrix.setCursor(init_x, init_y);
    matrix.print(SCREEN_INIT_TEXT);
    matrix.show();
}

void setText(String text, int *color, void (*callback)(String))
{
    matrix.fillScreen(0);
    matrix.setCursor(0, 0);
    matrix.clear();
    setTextColor(color[0], color[1], color[2]);
    matrix.setTextWrap(false);
    matrix.print(text);
    matrix.show();
    if (callback != NULL)
    {
        callback(text);
    }
}

void setBrightness(int brightness)
{
    matrix.setBrightness(brightness);
}

void moveCursor(String text, int cursor)
{
    matrix.clear();
    matrix.fillScreen(0);
    matrix.setCursor(cursor, 0);
    matrix.print(text);
    matrix.show();
}

void setTextColor(int r, int g, int b)
{
    matrix.setTextColor(matrix.Color(r, g, b));
}
