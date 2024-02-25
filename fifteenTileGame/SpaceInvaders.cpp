#include "Adafruit.h"
#include "SpaceInvaders.h"

extern int leftBtn;
extern int upBtn;
extern int downBtn;
extern int rightBtn;
extern int resetGameBtn;
extern int stopGameBtn;

extern unsigned long startTime;
extern unsigned long elapsedTime;

extern int inputNumber;
extern bool gameOver;

extern Adafruit_ST7789 tft;

void SpaceShipGame()
{

}

void SpaceShipLoop() {
  bool legaMove = false;
  //only process, if the input is onw of the 4 buttons
  inputNumber = checkInput();

  if (gameOver && inputNumber >= 0) {
    gameOver = false;

    drawGame();
  }

  if (inputNumber >= 0 && elapsedTime > 200) {
    startTime = millis();
    
    switch (inputNumber) {
      case 0:  //Left
        legaMove = LeftMove();
        break;
      case 1:  //Up
        legaMove = UpMove();
        break;
      case 2:  //Down
        legaMove = DownMove();
        break;
      case 3:  //Right
        legaMove = RightMove();
        break;
      case 4:  //reset
        drawGame();
        gameOver = false;
        break;
      case 5:
        gameOver = true;
        break;
      default:
        break;
    }

    if (legaMove) {
      drawGame();
    } else {
      //illegal move
      tft.drawRect(0, 0, 240, 240, ST77XX_RED);
      delay(500);
      tft.drawRect(0, 0, 240, 240, ST77XX_WHITE);
      delay(500);
      tft.drawRect(0, 0, 240, 240, ST77XX_RED);
      delay(500);
      tft.drawRect(0, 0, 240, 240, ST77XX_ORANGE);
    }

    inputNumber = -1;
  }
}

void drawGame()
{
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  tft.setCursor(4, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(4);
  tft.setTextWrap(true);
  tft.println("Spaceship Invaders");
}

//Left
bool LeftMove() {
  return false;
}

//Right
bool RightMove() {
  return false;
}
//Upwards
bool UpMove() {
  return false;
}
//Down
bool DownMove() {
  return false;
}