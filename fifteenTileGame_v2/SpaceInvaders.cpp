#include "Adafruit.h"
#include "SpaceInvaders.h"

extern int leftBtn;
extern int upBtn;
extern int downBtn;
extern int rightBtn;
extern int resetGameBtn;
extern int startBtn;

extern unsigned long startTime;
extern unsigned long elapsedTime;

extern int inputNumber;
extern bool gameOver;

extern Adafruit_ST7789 tft;

  uint16_t x0 = 105;
  uint16_t y0 = 210;

void SpaceShipGame() {
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  tft.setCursor(4, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(4);
  tft.setTextWrap(true);
  tft.println("Spaceship Invaders");

  //delay(1500);

  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);

  spawnSpaceJet(0, 0);
}

void SpaceShipLoop() {
  bool legaMove = false;
  inputNumber = checkInput();

  if (gameOver && inputNumber >= 0) {
    gameOver = false;

    SpaceShipGame();
  }

  //only process, if the input is one of the 4 action buttons
  if (inputNumber >= 0 && elapsedTime > 100) {
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
        redrawGame(0);
        gameOver = false;
        break;
      case 5:
        gameOver = true;
        break;
      default:
        break;
    }

    if (legaMove) {
      redrawGame(inputNumber);
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

//Called to refresh the screen
void redrawGame(uint16_t move_x = 0) {

  if(move_x == 0)
  {
    //left move
    spawnSpaceJet(-5, 0);
  }
  else if(move_x == 3){
    //right move
    spawnSpaceJet(5, 0);
  }

}

void spawnSpaceJet(uint16_t x, uint16_t y) {


  uint16_t w;
  uint16_t h = w = 30;

  //clear the section
  tft.fillRect(x0, y0, w, h, ST77XX_BLACK);

  x0 = x0 + x;
  y0 = y0 + y;
  //redraw the ship
  tft.fillRect(x0, y0, w, h, ST77XX_BLUE);
}

//Left
bool LeftMove() {
  return true;
}

//Right
bool RightMove() {
  return true;
}
//Upwards
bool UpMove() {
  return false;
}
//Down
bool DownMove() {
  return false;
}