#include "Adafruit_ST77xx.h"
#include "Adafruit.h"

#define TFT_CS 8    // if your display has CS pin
#define TFT_RST 10  // reset pin
#define TFT_DC 9    // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int resetGameBtn = 2;
int stopGameBtn = 3;

int jsxInput = A0;
int jsyInput = A1;

unsigned long startTime;
unsigned long elapsedTime;


int inputNumber;
bool gameOver = true;
void ClearScreen(){
  tft.drawRect(0, 0, 240, 240, ST77XX_BLACK);
}