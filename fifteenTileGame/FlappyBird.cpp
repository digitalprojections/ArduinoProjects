#include "HardwareSerial.h"
#include "Adafruit_ST77xx.h"
#include "Arduino.h"
#include "Adafruit.h"
#include "FlappyBird.h"
#include "FifteenTileGame.h"

#include "Sprite.h"
#include "Chym.h"
#include "Bar.h"

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


Chym player;
Bar bar;
Bar bar2;
int gameScore = 0;

boolean clicked = false;

void FlappyBirdGame() {
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  tft.setCursor(50, 30);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.println("Flappy Bird");

  delay(1000);

  player.respawn();
}

void FlappyBirdGameLoop() {

  inputNumber = checkInput();
  if(inputNumber == 3)
  {
    clicked = true;
  }
  player.update();
  bar.update();
  bar2.update();
  drawLCD();
}


void resetGame() {
  player.respawn();
  bar.setPos(0, 20);
  bar2.setPos(50, 30);
  gameScore = 0;
}


void drawLCD() {
  
  ClearScreen();

  if (!player.isDead()) {
    int ht1 = bar.hitTest(player.getX(), player.getY());
    int ht2 = bar2.hitTest(player.getX(), player.getY());
    int die = ht1 + ht2;
    if (die == 1) {
      // game over
      player.die();
    }

    if (clicked) {
      player.flyUp();
    } else {
      player.cancelJump();
    }
    player.render();

    bar.render();
    bar2.render();
  } else {
    tft.println("Game   Over!!!");
    if (clicked) {
      resetGame();
    }
  }

}
