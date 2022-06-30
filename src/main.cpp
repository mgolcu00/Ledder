#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "esp_system.h"
#include "connection.h"
#include "screen.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

unsigned long task_interval = 100;
unsigned long task_last_time = millis();
unsigned long delay_interval = 2500;
unsigned long delay_last_time = millis();
int flag = 0;
int key = 0;
int cursor = 0;
int text_pixel_length = 0;
int width = matrix.width();
String current_text = "";

void onTextChanged(String text)
{
  current_text = text;
  text_pixel_length = text.length() * 6;
  cursor = 0;
  flag = 0;
}

int **pomat;
int flag2 = 0;
void draw_point(int x, int y, int r, int g, int b)
{
  matrix.clear();
  int *color = new int[3];
  color[0] = r;
  color[1] = g;
  color[2] = b;
  matrix.drawPixel(x, y, matrix.Color(color[0], color[1], color[2]));
  matrix.show();
}

void draw_point(int x, int y, int c)
{
 
 // Serial.printf("%d %d %d\n", x, y, c);

  matrix.drawPixel(x, y, c);
 
}

// mesaj geldiğinde çalışacak
void callback(char *topic, byte *payload, unsigned int length)
{

  // if (strcmp(topic, "main/esp32/image") == 0)
  // {
  //   Serial.println("Message arrived in topic: main/esp32/image");
  //   Serial.print("Message:");
  //   for (int i = 0; i < length; i++)
  //   {
  //     Serial.print((char)payload[i]);
  //   }
  //   Serial.println("");
  //   matrix.clear();
  //   matrix.fillScreen(0);
  //   matrix.drawBitmap(0, 0, payload, matrix.width(), matrix.height(), 1);
  //   matrix.show();

  //   return;
  // }

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  if (strcmp(topic, out_topic) == 0)
  {

    JSONVar json;
    convertToJson(payload, length, json);
    JSONVar point = json["point"];
    int x = (int)point["x"];
    int y = (int)point["y"];
    int r = (int)point["r"];
    int g = (int)point["g"];
    int b = (int)point["b"];

    Serial.printf("x: %d, y: %d, r: %d, g: %d, b: %d\n", x, y, r, g, b);
    pomat[x][y] = matrix.Color(r, g, b);

    draw_point(x, y, r, g, b);
    flag2 = 1;
    return;
  }
  flag2 = 0;

  JSONVar json;
  convertToJson(payload, length, json);
  String text = (const char *)json["text"];
  int r = json["r"];
  int g = json["g"];
  int b = json["b"];

  Serial.println();
  Serial.println("-----------------------");
  int *color = new int[3];
  color[0] = r;
  color[1] = g;
  color[2] = b;
  setText(text, color, onTextChanged);
}

void scroll_with_millis()
{

  if (millis() - delay_last_time > delay_interval) // delay
  {
    flag = 1;
    delay_last_time = millis();
  }
  if (flag == 1)
  {
    if (width < text_pixel_length)
    {
      if ((cursor > width - text_pixel_length) && key == 0)
      {
        if (millis() - task_last_time > task_interval) // while
        {

          matrix.clear();
          matrix.fillScreen(0);
          matrix.setCursor(cursor, 0);
          matrix.print(current_text);
          matrix.show();
          cursor--;
          task_last_time = millis();
        }
      }
      else
      {
        if (key == 0)
        {
          flag = 0;
        }
        key = 1;
      }

      if (cursor != 1 && key == 1)
      {
        if (millis() - task_last_time > task_interval) // while
        {

          matrix.clear();
          matrix.fillScreen(0);
          matrix.setCursor(cursor, 0);
          matrix.print(current_text);
          matrix.show();
          cursor++;
          task_last_time = millis();
        }
      }
      else
      {
        if (key == 1)
        {
          flag = 0;
        }
        key = 0;
      }
    }
  }
}

void scroll_with_millis_task(void *pvParameters)
{

  for (;;)
  {
     scroll_with_millis();

  }
}

void network_task(void *pvParameters)
{

  for (;;)
  {
    client.loop();
  }
}

void set_tasks()
{
  BaseType_t xReturned;
  TaskHandle_t xHandle;
  TaskHandle_t xHandle2;

  xTaskCreatePinnedToCore(
      network_task,
      "network_task",
      16384,
      NULL,
      3,
      &xHandle,
      1);

  xTaskCreatePinnedToCore(
      scroll_with_millis_task,
      "scroll_task",
      16384,
      NULL,
      3,
      &xHandle2,
      1);
}

void setup()
{

  Serial.begin(115200);

  pomat = new int *[matrix.width()];
  for (int i = 0; i < matrix.width(); i++)
  {
    pomat[i] = new int[matrix.height()];
  }
  for (size_t i = 0; i < matrix.width(); i++)
  {
    for (size_t j = 0; j < matrix.height(); j++)
    {
      pomat[i][j] = matrix.Color(0, 0, 0);
    }
  }
  

  // configure neoPixel table
  initMatrix();
  // connect wifi
  connectToWifi();
  // set mqtt callback
  client.setCallback(callback);
  connectToMQTT();
  // set tasks
  set_tasks();
}

void loop()
{
  delay(100);
}
