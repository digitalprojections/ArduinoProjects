#include "Adafruit.h"

#define TFT_CS 10  // if your display has CS pin
#define TFT_RST 9  // reset pin
#define TFT_DC 8   // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int leftBtn = 5;
int upBtn = 2;
int downBtn = 4;
int rightBtn = 3;
int resetGameBtn = 7;
int stopGameBtn = 6;

unsigned long startTime;
unsigned long elapsedTime;

int inputNumber;
bool gameOver = true;