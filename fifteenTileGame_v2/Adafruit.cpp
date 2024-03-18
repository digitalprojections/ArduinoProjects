#include "Adafruit.h"

#define TFT_CS 10  // if your display has CS pin
#define TFT_RST 9  // reset pin
#define TFT_DC 8   // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int leftBtn = 7;                                                                                                        ;
int upBtn = 4;
int downBtn = 2;
int rightBtn = 6;
int resetGameBtn = 3;
int startBtn = 5;

unsigned long startTime;
unsigned long elapsedTime;

int inputNumber;
bool gameOver = true;