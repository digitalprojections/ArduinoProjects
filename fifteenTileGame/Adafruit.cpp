#include "Adafruit.h"

#define TFT_CS 8  // if your display has CS pin
#define TFT_RST 10  // reset pin
#define TFT_DC 9   // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int leftBtn = 4                                                                                                        ;
int upBtn = 7;
int downBtn = 2;
int rightBtn = 11;
int resetGameBtn = 3;
int stopGameBtn = 5;

unsigned long startTime;
unsigned long elapsedTime;

int inputNumber;
bool gameOver = true;