#include "Arduino.h"
#include "Adafruit.h"
#include "SpaceInvaders.h"

extern Adafruit_ST7789 tft;

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

unsigned long bulletSpawnTime;
unsigned long bulletTravelTime;

unsigned long gameStartTime;
unsigned long alienMoveTime;

bool bullet = false;
bool alienInvasion = false;
bool aliendDirectionRight = false;

uint16_t alienStart = 3000;
uint16_t alienStartCounter = 1;

//alien
uint16_t alienX = 105;  //alien origin
uint16_t alienY = 30;

//spaceship
uint16_t x0 = 105;  //spaceship origin
uint16_t y0 = 210;

uint16_t tx = 0;  //right engine triangle
uint16_t ty = 0;

uint16_t lx = 0;  //left engine triange
uint16_t ly = 0;

uint16_t bx = 0;  //bullet pixel
uint16_t by = 0;

uint16_t bsInterval = 10;  //bullet speed needed, because if moved every cycle, the bullet moves too fast
uint16_t bs = 0;

void SpaceShipGame() {
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  tft.setCursor(4, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(4);
  tft.setTextWrap(true);
  tft.println("Space Invaders");

  delay(1000);

  gameStartTime = millis();

  //clear screen
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);

  //initial draw. once
  drawSpaceJet(0, 0);
}

void SpaceShipLoop() {
  bool goodMove = false;
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
        goodMove = LeftMove();
        break;
      case 1:  //Up, not supported
        goodMove = UpMove();
        break;
      case 2:  //Down. not supported
        goodMove = DownMove();
        break;
      case 3:  //Right
        goodMove = RightMove();
        break;
      case 4:  //reset
        gameOver = false;
        break;
      case 5:
        gameOver = true;
        break;
      default:
        break;
    }

    if (goodMove) {
      redrawGame(inputNumber);
    } else if (!bullet) {
      IllegalMove();
    }

    inputNumber = -1;
  }


  DrawAlienShips();
}

//Called to refresh the screen
void redrawGame(uint16_t move_x = 0) {

  if (move_x == 0) {
    //left move
    drawSpaceJet(-5, 0);
  } else if (move_x == 3) {
    //right move
    drawSpaceJet(5, 0);
  } else if (move_x == 4) {
    Serial.print("move_x: ");
    Serial.println(move_x);
  }
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
  if (!bullet) {
    //shoot a bullet
    bx = x0;
    by = y0 - 15;
    bullet = true;
    bulletSpawnTime = millis();
  }
  return false;
}
//Down
bool DownMove() {
  return false;
}


void ShootABullet() {

  bulletTravelTime = millis() - bulletSpawnTime;

  if (bullet && bulletTravelTime > bsInterval && by > 0) {
    bulletSpawnTime = millis();
    tft.drawPixel(bx, by, ST77XX_BLACK);
    by = by - 1;
    tft.drawPixel(bx, by, ST77XX_RED);
    if (by <= 0) {
      bullet = false;
      tft.drawLine(0, 0, 240, 0, ST77XX_BLACK);
    }
  }
}


void drawSpaceJet(uint16_t x, uint16_t y) {

  uint16_t w;
  uint16_t h = w = 25;

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

void DrawAlienShips() {
  if (alienInvasion) {
    uint16_t w;
    uint16_t h = w = 15;

    //clear the section
    tft.fillRect(alienX - 7, alienY - 7, w, h, ST77XX_BLACK);

    //redraw the ship
    int size = 5;

    alienMoveTime = millis() - gameStartTime;
    if (aliendDirectionRight && alienX < 225) {
      alienX++;
      if (alienX == 225) {
        aliendDirectionRight = false;
      }
    } else if (alienX > 15) {
      alienX--;
      if (alienX == 15) {
        aliendDirectionRight = true;
      }
    }

    //check for collision
    if (bx >= alienX - size && bx <= alienX + size && by >= alienY - size && by <= alienY + size) {
      gameStartTime = millis();
      alienInvasion = false;
    } else {
      //draw 3 red legs
      tft.drawLine(alienX - size, alienY - size, alienX + size, alienY + size, ST77XX_YELLOW);
      tft.drawLine(alienX - size, alienY, alienX + size, alienY, ST77XX_YELLOW);
      tft.drawLine(alienX - size, alienY + size, alienX + size, alienY - size, ST77XX_YELLOW);
      //draw body
      tft.fillCircle(alienX, alienY, size, ST77XX_RED);
    }

  } else {
    //check time to start invasion
    
    int coundDown = millis() - gameStartTime;
    
      //alienStartCounter++;
      CountDown((-3 + (coundDown / 1000)) * -1);
    
    if (coundDown > alienStart) {
      //alienStartCounter = 1;
      CountDown(0);
      delay(100);
      alienInvasion = true;
    }
  }
}