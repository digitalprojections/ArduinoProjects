#include "Adafruit_ST77xx.h"
#include "Adafruit.h"

#define TFT_CS 8    // if your display has CS pin
#define TFT_RST 10  // reset pin
#define TFT_DC 9    // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//int leftBtn = 4;
//int upBtn = 7;
//int downBtn = A0;
//int rightBtn = 2;
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

void IllegalMove() {
  //illegal move
  tft.drawRect(0, 0, 240, 240, ST77XX_RED);
  delay(100);
  tft.drawRect(0, 0, 240, 240, ST77XX_WHITE);
  delay(100);
  tft.drawRect(0, 0, 240, 240, ST77XX_RED);
  delay(100);
  tft.drawRect(0, 0, 240, 240, ST77XX_ORANGE);
}


int counter = 0;

void CountDown(int count) {
  //to stop filckering effect of repeated action
  if (count != counter) {
    counter = count;
    tft.fillRect(100, 100, 50, 50, ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(110, 100);
    tft.println(count);
    if (count == 0) {
      tft.fillRect(100, 100, 50, 50, ST77XX_BLACK);
    }
  }
}

//Call to update game starts at the top
void UpdateGameStats(uint8_t livesLeft, uint8_t hitCount, uint8_t missCount, int8_t wavesLeft = -1) {
  tft.fillRect(0, 0, 240, 29, ST77XX_BLACK);
  tft.setCursor(4, 2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print("Lives: ");
  tft.print(livesLeft);  //show life count


  if (wavesLeft >= 0) {
    tft.setCursor(120, 2);
    tft.setTextColor(ST77XX_MAGENTA);
    tft.setTextSize(1);
    tft.setTextWrap(true);
    tft.print("Waves left: ");
    tft.print(wavesLeft);
  }

  tft.setCursor(4, 10);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print("hits: ");
  tft.print(hitCount);
  tft.setCursor(120, 10);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print("misses: ");
  tft.print(missCount);
}