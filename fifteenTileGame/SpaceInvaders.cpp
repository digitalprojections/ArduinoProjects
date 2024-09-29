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

bool bullet = false;

extern Adafruit_ST7789 tft;

uint16_t x0 = 105;
uint16_t y0 = 210;

uint16_t tx = 0;  //right engine triangle
uint16_t ty = 0;

uint16_t lx = 0;  //left engine triange
uint16_t ly = 0;

uint16_t bx = 0;  //bullet pixel
uint16_t by = 0;

uint16_t bsInterval = 100; //bullet speed needed, because if moved every cycle, the bullet moves too fast
uint16_t bs = 0;

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

  ShootABullet();

  //gameover check
  if (gameOver && inputNumber >= 0) {
    gameOver = false;

    SpaceShipGame();
  }

  //if the input a direction button
  if (inputNumber >= 0 && elapsedTime > 100) {
    startTime = millis();

    switch (inputNumber) {
      case 0:  //Left
        legaMove = LeftMove();
        break;
      case 1:  //Up, not supported
        legaMove = UpMove();
        break;
      case 2:  //Down. not supported
        legaMove = DownMove();
        break;
      case 3:  //Right
        legaMove = RightMove();
        break;
      case 4:  //reset
        redrawGame(4);
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
      IllegalMove();
    }

    inputNumber = -1;
  }
}

//Called to refresh the screen
void redrawGame(uint16_t move_x = 0) {

  if (move_x == 0) {
    //left move
    spawnSpaceJet(-5, 0);
  } else if (move_x == 3) {
    //right move
    spawnSpaceJet(5, 0);
  } else if (move_x == 4) {
    Serial.print("move_x: ");
    Serial.println(move_x);
  }
}

void spawnSpaceJet(uint16_t x, uint16_t y) {


  uint16_t w;
  uint16_t h = w = 25;

  // sekin ishlaydigan kod:
  // x0 = x0 + x;
  // y0 = y0 + y;

  // uint16_t tx = x0;
  // uint16_t ty = y0;

  // for (uint16_t i = 0; i < 25; i++) {
  //   for (uint16_t j = 0; j < 25; j++) {
  //     if (spaceShip[i][j] == 1) {
  //       x0 = tx + i;
  //       y0 = ty + j;
  //       tft.drawPixel(x0, y0, ST77XX_BLUE);
  //       Serial.println("x0: ");
  //       Serial.println(x0);
  //     }
  //   }
  // }
  // x0 = 105;
  // y0 = 210;


  //clear the section
  tft.fillCircle(x0, y0, 15, ST77XX_BLACK);
  tft.fillTriangle(lx, ly, lx - 5, ly + 10, lx + 5, ly + 10, ST77XX_BLACK);
  tft.fillTriangle(tx, ty, tx - 5, ty + 10, tx + 5, ty + 10, ST77XX_BLACK);



  x0 = x0 + x;
  y0 = y0 + y;
  //redraw the ship
  tft.fillCircle(x0, y0, 15, ST77XX_BLUE);

  //left engine
  lx = x0 - 12;
  ly = y0 + 10;
  tft.fillTriangle(lx, ly, lx - 5, ly + 10, lx + 5, ly + 10, ST77XX_ORANGE);

  //right engine
  tx = x0 + 13;
  ty = y0 + 10;
  tft.fillTriangle(tx, ty, tx - 5, ty + 10, tx + 5, ty + 10, ST77XX_ORANGE);
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
  //shoot a bullet
  bx = x0;
  by = y0 - 15;
  bullet = true;
  return false;
}
//Down
bool DownMove() {
  return false;
}


void ShootABullet() {

  bs++;

  if (bullet && bs > bsInterval) {
    bs = 0;
    tft.drawPixel(bx, by, ST77XX_BLACK);
    by = by - 1;
    tft.drawPixel(bx, by, ST77XX_RED);
  }
}